#include "stdwx.h"
#include "PerspectiveFilter.h"
#include <opencv2/opencv.hpp>
#include <IFloorCameraSettings.h>

IMPLEMENT_DYNAMIC_CLASS(PerspectiveFilter, Filter);

PerspectiveFilter::PerspectiveFilter()
{
	cvsrc.resize(4, cv::Point2f(0.0f, 0.0f));
	cvdst.resize(4, cv::Point2f(0.0f, 0.0f));

	xMax = xMin = yMin = yMax = 0.0f;

}

PerspectiveFilter::~PerspectiveFilter()
{
	
}

wxString PerspectiveFilter::GetName() const
{
	return _("Perspective");
}

#if defined(HAVE_CUDA)
bool PerspectiveFilter::KernelGPU()
{
	if(!Translate()) return false;
	cv::Mat translateMat(translate);
	m_src->download(src);
	//cv::gpu::warpPerspective(*m_src, m_dst, translateMat, cv::Size(xMax - xMin, yMax - yMin));
	cv::warpPerspective(src, dst, translateMat, cv::Size(xMax - xMin, yMax - yMin));
	m_dst.upload(dst);
	return true;
}
#endif

	//original code
	/**
	*    A  +-------------+  B
	*      /               \
	*     /                 \
	*    /                   \
	* D +-------------------- +  C
	*/
    //CvPoint2D32f cvsrc[4]; 
    //CvPoint2D32f cvdst[4]; 
    //CvMat* translate = cvCreateMat(3,3, CV_32FC1); 
    //cvSetZero(translate); 

    //cvdst[0].x = 0; 
    //cvdst[0].y = 0; 
    //cvdst[1].x = width; 
    //cvdst[1].y = 0; 
    //cvdst[2].x = width; 
    //cvdst[2].y = height; 
    //cvdst[3].x = 0; 
    //cvdst[3].y = height; 

    //cvsrc[0].x = A.x; 
    //cvsrc[0].y = A.y; 
    //cvsrc[1].x = B.x; 
    //cvsrc[1].y = B.y; 
    //cvsrc[2].x = C.x; 
    //cvsrc[2].y = C.y; 
    //cvsrc[3].x = D.x; 
    //cvsrc[3].y = D.y; 

    //cvWarpPerspectiveQMatrix(cvsrc, cvdst, translate);  // calculate homography 
    //cvWarpPerspective(cvImage, cvImageTemp, translate); 
    //swapTemp();
    //flagImageChanged();
    //cvReleaseMat(&translate); 

bool PerspectiveFilter::Translate()
{
	if (cvdst.empty() || cvdst.size() < 4)
		return false;

	xMax = cvdst[0].x;
	xMin = cvdst[0].x;
	yMin = cvdst[0].y;
	yMax = cvdst[0].y;

	for (int i = 1; i < 4; i++)
	{
		xMax = wxMax(xMax, cvdst[i].x);
		xMin = wxMin(xMin, cvdst[i].x);
		yMax = wxMax(yMax, cvdst[i].y);
		yMin = wxMin(yMin, cvdst[i].y);
	}

	if (xMin == xMax || yMin == yMax)
		return false;

	_translate = cv::getPerspectiveTransform(cvsrc, cvdst);

	return !_translate.empty();
}

bool PerspectiveFilter::Kernel()
{
	if(!Translate()) return false;
	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(cv::Size(static_cast<int>(xMax - xMin), static_cast<int>(yMax - yMin)), source.type());
	}
	//cvWarpPerspective(source, destination, translate); 
	cv::warpPerspective(source, destination, _translate, destination.size());
	return true;
}

void PerspectiveFilter::SetSettings(const IFloorCameraSettings& settings)
{
	cvsrc[0] = cv::Point2f(settings.TopLeftX, settings.TopLeftY);
	cvsrc[1] = cv::Point2f(settings.TopRightX, settings.TopRightY);
	cvsrc[2] = cv::Point2f(settings.BottomRightX, settings.BottomRightY);
	cvsrc[3] = cv::Point2f(settings.BottomLeftX, settings.BottomLeftY);

	cvdst[0] = cv::Point2f(0.0f, 0.0f);
	cvdst[1] = cv::Point2f((float)settings.CamWidth, 0.0f);
	cvdst[2] = cv::Point2f((float)settings.CamWidth, (float)settings.CamHeight);
	cvdst[3] = cv::Point2f(0.0f, (float)settings.CamHeight);
}
wxFORCE_LINK_THIS_MODULE(PerspectiveFilter);