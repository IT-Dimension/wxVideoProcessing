#ifndef _TRACKING_H
#define _TRACKING_H

#include <list>
#include <map>

#include "ContourFinder.h"

class MOTION_DETECTOR_CORE_API BlobTracker
{
public:
	BlobTracker();
	~BlobTracker();

	// Assigns IDs to each blob in the contourFinder
	void track(ContourFinder* newBlobs);
	bool isCalibrating;
	int MIN_MOVEMENT_THRESHOLD;
	const iFloorBlobVector & getTrackedBlobs() const;

private:
	int trackKnn(ContourFinder *newBlobs, iFloorBlob *track, int k, double thresh);

private:
	int							IDCounter;		//counter of last blob
	int							fightMongrel;
	iFloorBlobVector			trackedBlobs;	//tracked blobs
	std::map<int, iFloorBlob>	calibratedBlobs;
};

#endif
