#include "stdwx.h"
#include "ShapeFilter.h"
#include <opencv2/opencv.hpp>

// ----  initialization of non-integral constants  ----------------------------


const char *ShapeFilter::TRACKBAR_LABEL_BLUR 		= "blur";
const char *ShapeFilter::PARAMETER_BLUR		= "blur";

const char *ShapeFilter::TRACKBAR_LABEL_LEVEL 		= "level";
const char *ShapeFilter::PARAMETER_LEVEL		= "level";


// ----  implementations  -----------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(ShapeFilter, Filter);

ShapeFilter::ShapeFilter()
{
	blurLevel = DEFAULT_BLUR_LEVEL;
	levelLevel = DEFAULT_LEVEL_LEVEL;
}


ShapeFilter::~ShapeFilter()
{

}

wxString ShapeFilter::GetName() const
{
	return _("Shape");
}

bool ShapeFilter::Kernel()
{
	if (source.empty())
	{
		return false;
	}

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	if (buffer.empty())
	{
		buffer = cv::Mat::zeros(source.size(), source.type());
	}

	destination.setTo(cv::Scalar::all(0));

	// create the unsharp mask using a linear average filter
	int blurParameter = blurLevel * 2 + 1;
	cv::blur(source, buffer, cv::Size(blurParameter, blurParameter));
	
	//cvSub(source, buffer, buffer);
//	cvAbsDiff(source, buffer, buffer);

	cv::Mat tempSource = source.clone();
	cv::threshold(tempSource, tempSource, levelLevel, 255, cv::THRESH_TOZERO);
	cv::Canny(tempSource, destination, (float)blurParameter, (float)blurParameter * 3, 3);

	return true;

/*
	//cvSmooth(buffer, buffer, CV_MEDIAN, 3, 1);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0; 
	CvSeq *result;
	cvFindContours( buffer, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE );
		
	for( ; contours != 0; contours = contours->h_next )	
	{
		int count = contours->total; // This is number point in contour

		// First we check to see if this contour looks like a square.. 

        result = cvApproxPoly( contours, sizeof(CvContour), storage,
            CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );

		CvRect r = cvBoundingRect(result);
		double area = fabs(cvContourArea(result,CV_WHOLE_SEQ));
			cvDrawContours(destination, result, CV_RGB(255,255,255), CV_RGB(255,255,255), 3, 2, CV_FILLED);
        if( result->total > 2 &&
             area > 20 && r.width < 50 && r.height < 50)
        {


		}

		//contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );


	}
	cvReleaseMemStorage(&storage);
*/
}

wxFORCE_LINK_THIS_MODULE(ShapeFilter);