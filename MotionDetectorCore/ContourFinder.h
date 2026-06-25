#ifndef IFLOORCONTOUR_FINDER_H
#define IFLOORCONTOUR_FINDER_H

#if defined(__WXMSW__)
#include <opencv2/opencv.hpp>
#include <vector>
#include "iFloorBlob.h"
#include "MotionDetectorCorePlugin.h"

struct SeqArea
{
	SeqArea() 
		: Area(0) {}
	SeqArea(const std::vector<cv::Point>& seq, double area)
		: Seq(seq)
		, Area(area)
	{

	}
	SeqArea(const SeqArea & seqArea)
		: Seq(seqArea.Seq)
		, Area(seqArea.Area) 
	{

	}
	SeqArea & operator=(const SeqArea & seqArea)
	{
		if (this != &seqArea)
		{
			Seq = seqArea.Seq;
			Area = seqArea.Area;
		}
		return *this;
	}
	std::vector<cv::Point> Seq;
	double Area;
};

class MOTION_DETECTOR_CORE_API ContourFinder
{
public:
	ContourFinder();
	~ContourFinder();

	int findContours( /*ofxCvGrayscaleImage& input,*/
		cv::Mat img,
		int minArea, int maxArea,
		size_t nConsidered, bool bFindHoles,
		bool bUseApproximation = true);
	// approximation = don't do points for all points of the contour, if the contour runs
	// along a straight line, for example...

	iFloorBlobVector	blobs;	// the blobs, in a std::vector...
protected:

	// this is stuff, not for general public to touch -- we need
	// this to do the blob detection, etc.
	//ofxCvGrayscaleImage inputCopy;

	// internally, we find cvSeqs, they will become blobs.
	std::vector<SeqArea> cvSeqBlobs;

	// important!!
	void				reset();
	void FillBlob(SeqArea& seqBlob, iFloorBlob& blob);
};

#endif
#endif