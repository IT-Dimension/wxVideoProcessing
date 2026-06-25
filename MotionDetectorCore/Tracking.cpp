#include "stdwx.h"
#include "Tracking.h"

#if defined(USE_VLD)
#include <vld.h>
#endif

BlobTracker::BlobTracker()
{
	IDCounter = 0;
	isCalibrating = false;
}

BlobTracker::~BlobTracker(){

}

// Assigns IDs to each blob in the contourFinder
void BlobTracker::track(ContourFinder* newBlobs)
{
	//initialize ID's of all blobs
	for(size_t i=0; i<newBlobs->blobs.size(); i++)
		newBlobs->blobs[i].id=-1;
	
	//go through all tracked blobs to compute nearest new point
	for (size_t i=0; i<trackedBlobs.size(); i++)
	{
		/******************************************************************
		 * *****************TRACKING FUNCTION TO BE USED*******************
		 * Replace 'trackKnn(...)' with any function that will take the
		 * current track and find the corresponding track in the newBlobs
		 * 'winner' should contain the index of the found blob or '-1' if
		 * there was no corresponding blob
		 *****************************************************************/
		int winner = trackKnn(newBlobs, &(trackedBlobs[i]), 3, 0);

		if (winner==-1) //track has died, mark it for deletion
		{
			trackedBlobs[i].id = -1;
		}
		else //still alive, have to update
		{
			//if winning new blob was labeled winner by another track\
			//then compare with this track to see which is closer
			if (newBlobs->blobs[winner].id!=-1)
			{
				//find the currently assigned blob
				size_t j = 0; //j will be the index of it
				for (j = 0; j < trackedBlobs.size(); j++)
				{
					if (trackedBlobs[j].id==newBlobs->blobs[winner].id) break;
				}

				if (j==trackedBlobs.size())//got to end without finding it
				{
					newBlobs->blobs[winner].id = trackedBlobs[i].id;
					trackedBlobs[i] = newBlobs->blobs[winner];
				}
				else //found it, compare with current blob
				{
					double x = newBlobs->blobs[winner].centroid.x;
					double y = newBlobs->blobs[winner].centroid.y;
					double xOld = trackedBlobs[j].centroid.x;
					double yOld = trackedBlobs[j].centroid.y;
					double xNew = trackedBlobs[i].centroid.x;
					double yNew = trackedBlobs[i].centroid.y;
					double distOld = (x-xOld)*(x-xOld)+(y-yOld)*(y-yOld);
					double distNew = (x-xNew)*(x-xNew)+(y-yNew)*(y-yNew);

					//if this track is closer, update the ID of the blob
					//otherwise delete this track.. it's dead
					if (distNew < distOld) //update
					{
						newBlobs->blobs[winner].id = trackedBlobs[i].id;
						trackedBlobs[j].id = -1;
//------------------------------------------------------------------------------
					}
					else //delete
					{
						trackedBlobs[i].id = -1;
					}
				}
			}
			else //no conflicts, so simply update
			{
				newBlobs->blobs[winner].id = trackedBlobs[i].id;
			}
		}
	}

	//--Update All Current Tracks
	//remove every track labeled as dead (ID='-1')
	//find every track that's alive and copy it's data from newBlobs

	auto new_end = std::remove_if(trackedBlobs.begin(), trackedBlobs.end(),
		[](const iFloorBlob& blob) 	{ return blob.id == -1; });
	trackedBlobs.erase(new_end, trackedBlobs.end());

	for (size_t i = 0; i < trackedBlobs.size(); i++)
	{
		for (size_t j = 0; j < newBlobs->blobs.size(); j++)
		{
			if (trackedBlobs[i].id == newBlobs->blobs[j].id)
			{
				//update track
				iFloorPoint tempLastCentroid = trackedBlobs[i].centroid; // assign the new centroid to the old
				trackedBlobs[i] = newBlobs->blobs[j];
				trackedBlobs[i].lastCentroid = tempLastCentroid;
			}
		}
	}
	//--Add New Living Tracks
	//now every new blob should be either labeled with a tracked ID or\
	//have ID of -1... if the ID is -1... we need to make a new track
	for(size_t i=0; i<newBlobs->blobs.size(); i++)
	{
		if(newBlobs->blobs[i].id==-1)
		{
			//add new track
			newBlobs->blobs[i].id=IDCounter++;
			trackedBlobs.push_back(newBlobs->blobs[i]);
		}
	}
}

const iFloorBlobVector & BlobTracker::getTrackedBlobs() const
{
	return trackedBlobs;
}

/*************************************************************************
* Finds the blob in 'newBlobs' that is closest to the trackedBlob with index
* 'ind' according to the KNN algorithm and returns the index of the winner
* newBlobs	= list of blobs detected in the latest frame
* track		= current tracked blob being tested
* k			= number of nearest neighbors to consider\
*			  1,3,or 5 are common numbers..\
*			  must always be an odd number to avoid tying
* thresh	= threshold for optimization
**************************************************************************/

int BlobTracker::trackKnn(ContourFinder *newBlobs, iFloorBlob *track, int k, double thresh = 0)
{

	int winner = -1; //initially label track as '-1'=dead
	if ((k%2)==0) k++; //if k is not an odd number, add 1 to it

	//if it exists, square the threshold to use as square distance
	if (thresh>0)
		thresh *= thresh;

	//list of neighbor point index and respective distances
	std::list<std::pair<int,double> > nbors;
	std::list<std::pair<int,double> >::iterator iter;

	//find 'k' closest neighbors of testpoint
	double x, y, xT, yT, dist;
	for (size_t i=0; i<newBlobs->blobs.size(); i++)
	{
		x = newBlobs->blobs[i].centroid.x;
		y = newBlobs->blobs[i].centroid.y;

		xT = track->centroid.x;
		yT = track->centroid.y;
		dist = (x-xT)*(x-xT)+(y-yT)*(y-yT);

		if (dist<=thresh)//it's good, apply label if no label yet and return
		{
			winner = i;
			return winner;
		}

		/****************************************************************
		* check if this blob is closer to the point than what we've seen
		*so far and add it to the index/distance list if positive
		****************************************************************/

		auto nborsEnd = nbors.end();
		//search the list for the first point with a longer distance
		for (iter=nbors.begin(); iter != nborsEnd && dist >= iter->second; iter++);

		if ((iter != nborsEnd) || (nbors.size()<(size_t)k)) //it's valid, insert it
		{
			nbors.insert(iter, 1, std::pair<int, double>(i, dist));
			//too many items in list, get rid of farthest neighbor
			if (nbors.size()>(size_t)k)
				nbors.pop_back();
		}
	}

	/********************************************************************
	* we now have k nearest neighbors who cast a vote, and the majority
	* wins. we use each class average distance to the target to break any
	* possible ties.
	*********************************************************************/

	// a mapping from labels (IDs) to count/distance
	std::map<int, std::pair<int, double> > votes;

	//remember:
	//iter->first = index of newBlob
	//iter->second = distance of newBlob to current tracked blob
	auto nborsEnd = nbors.end();
	for (iter = nbors.begin(); iter != nborsEnd; iter++)
	{
		//add up how many counts each neighbor got
		int count = ++(votes[iter->first].first);
		double dist = (votes[iter->first].second+=iter->second);

		/* check for a possible tie and break with distance */
		if (count>votes[winner].first || count==votes[winner].first
			&& dist<votes[winner].second)
		{
			winner = iter->first;
		}
	}

	return winner;
}
