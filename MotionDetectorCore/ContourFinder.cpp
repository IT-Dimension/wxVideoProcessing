#include "stdwx.h"
#include "ContourFinder.h"
#include <algorithm>

static int qsort_carea_compare(void const * _a, void const * _b)
{
	// pointers, ugh.... sorry about this
	auto a = reinterpret_cast<const SeqArea *>(_a);
	auto b = reinterpret_cast<const SeqArea *>(_b);
	// use opencv to calc size, then sort based on size
	float areaa = a->Area;
	float areab = b->Area;
	// note, based on the -1 / 1 flip
	// we sort biggest to smallest, not smallest to biggest
	auto out = areaa > areab ? -1 : 1;
	return out;
}

ContourFinder::ContourFinder()
{
	reset();
}

ContourFinder::~ContourFinder()
{
}

void ContourFinder::reset()
{
	blobs.clear();
}

void ContourFinder::FillBlob(SeqArea& seqBlob, iFloorBlob& blob)
{
	if (seqBlob.Seq.empty()) return;

	cv::Point oldPt(0, 0);
	size_t totalPoints = seqBlob.Seq.size();

	for (size_t j = 0; j < totalPoints; ++j)
	{
		cv::Point pt = seqBlob.Seq[j];

		switch (j)
		{
		case 0:
			blob.pts.push_back(iFloorPoint(pt.x, pt.y));
			break;
		default:
	
			if ((abs(oldPt.x - pt.x) + abs(oldPt.y - pt.y)) > 5 || (j == totalPoints - 1))
			{
				blob.pts.push_back(iFloorPoint(pt.x, pt.y));
			}
			break;
		}
		oldPt = pt;
	}
}

int ContourFinder::findContours(/*ofxCvGrayscaleImage& input,*/
								cv::Mat img,
								int minArea,
								int maxArea,
								size_t nConsidered,
								bool bFindHoles,
								bool bUseApproximation)
{
	reset();

	// opencv will clober the image it detects contours on, so we want to
	// copy it into a copy before we detect contours. That copy is allocated
	// if necessary (necessary = (a) not allocated or (b) wrong size)
	// so be careful if you pass in different sized images to "findContours"
	// there is a performance penalty, but we think there is not a memory leak
	// to worry about better to create mutiple contour finders for different
	// sizes, ie, if you are finding contours in a 640x480 image but also a
	// 320x240 image better to make two ContourFinder objects then to use
	// one, because you will get penalized less.

	//if( inputCopy.width == 0 ) {
	//	inputCopy.allocate( input.width, input.height );
	//	inputCopy = input;
	//} else {
	//	if( inputCopy.width == input.width && inputCopy.height == input.height ) {
	//		inputCopy = input;
	//	} else {
	//		// we are allocated, but to the wrong size --
	//		// been checked for memory leaks, but a warning:
	//		// be careful if you call this function with alot of different
	//		// sized "input" images!, it does allocation every time
	//		// a new size is passed in....
	//		//inputCopy.clear();
	//		inputCopy.allocate( input.width, input.height );
	//		inputCopy = input;
	//	}
	//}

	if (img.empty()) return 0;

	cv::Mat grayImg;
	if (img.channels() == 3) {
		cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
	}
	else {
		grayImg = img.clone(); 
	}


	std::vector<std::vector<cv::Point>> all_contours;

	int retrieve_mode = bFindHoles ? cv::RETR_LIST : cv::RETR_EXTERNAL;
	int approx_mode = bUseApproximation ? cv::CHAIN_APPROX_SIMPLE : cv::CHAIN_APPROX_NONE;

	cv::findContours(grayImg, all_contours, retrieve_mode, approx_mode);

	cvSeqBlobs.clear();
	cvSeqBlobs.reserve(all_contours.size());

	for (const auto& contour : all_contours)
	{
		std::vector<cv::Point> approx_contour;
		cv::approxPolyDP(contour, approx_contour, 3.0, true);

		double area = std::fabs(cv::contourArea(approx_contour));
		if ((area >= minArea) && (area < maxArea))
		{
			cvSeqBlobs.push_back(SeqArea(approx_contour, area));
		}
	}

	std::sort(cvSeqBlobs.begin(), cvSeqBlobs.end(), [](const SeqArea& a, const SeqArea& b) {
		return a.Area > b.Area;
		});

	size_t nCvSeqsFound = cvSeqBlobs.size();
	nCvSeqsFound = std::min(nConsidered, nCvSeqsFound);

	blobs.resize(nCvSeqsFound);

	for (size_t i = 0; i < nCvSeqsFound; ++i)
	{
		SeqArea& seqBlob = cvSeqBlobs[i];
		iFloorBlob& blob = blobs[i];

		cv::Moments myMoments = cv::moments(seqBlob.Seq);

		cv::Rect rect = cv::boundingRect(seqBlob.Seq);
		blob.boundingRect.x = rect.x;
		blob.boundingRect.y = rect.y;
		blob.boundingRect.width = rect.width;
		blob.boundingRect.height = rect.height;

		cv::RotatedRect box = cv::minAreaRect(seqBlob.Seq);

		blob.angleBoundingRect.x = box.center.x;
		blob.angleBoundingRect.y = box.center.y;
		blob.angleBoundingRect.width = box.size.height;
		blob.angleBoundingRect.height = box.size.width;
		blob.angle = box.angle;

		blob.area = seqBlob.Area;

		if (myMoments.m00 != 0) {
			blob.centroid.x = (myMoments.m10 / myMoments.m00);
			blob.centroid.y = (myMoments.m01 / myMoments.m00);
		}
		else {
			blob.centroid.x = 0;
			blob.centroid.y = 0;
		}

		blob.lastCentroid.x = 0;
		blob.lastCentroid.y = 0;

		FillBlob(seqBlob, blob);
	}
	
	return static_cast<int>(blobs.size());
}