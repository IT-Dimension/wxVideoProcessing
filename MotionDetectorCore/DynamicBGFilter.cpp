#include "stdwx.h"
#include "DynamicBGFilter.h"

#if defined(__LINUX__)
#include "LinuxUtils.h"
#endif
// LINUX_

IMPLEMENT_DYNAMIC_CLASS(DynamicBGFilter, Filter);

DynamicBGFilter::DynamicBGFilter()
{
	m_bLearnBackground = false;
	m_bTrackDark = false;
	m_LearnRate = 400;
	m_CameraExposureTime = 2200;
#if defined(HAVE_CUDA)
	bStart = false;
#endif

	m_ExposureStartTime = timeGetTime();
	XS_SERIALIZE(m_bTrackDark, wxT("TrackDark"));
	XS_SERIALIZE(m_LearnRate, wxT("LearnRate"));
	XS_SERIALIZE(m_bLearnBackground, wxT("LearnBackground"));
	XS_SERIALIZE(m_CameraExposureTime, wxT("CameraExposureTime"));

}

DynamicBGFilter::~DynamicBGFilter()
{

}

wxString DynamicBGFilter::GetName() const
{
	return _("Dynamic BG");
}

#if defined(HAVE_CUDA)
bool DynamicBGFilter::KernelGPU()
{
	if ((int)(timeGetTime() - m_ExposureStartTime) > m_CameraExposureTime)
		m_bLearnBackground = true;

	//Capture full background
	if (m_bLearnBackground)
	{
		m_src->convertTo(m_matFloatBgImg, CV_32FC1);
		m_bLearnBackground = false;
		bStart = true;
		m_ExposureStartTime = timeGetTime();
	}
	if(!bStart) 
		return false;
	//step1
	m_src->convertTo(m_matFloatBgImgTemp, CV_32FC1);
	cv::Scalar alpha = (float)m_LearnRate * 0.001f;
	cv::Scalar beta = 1.0f - (float)m_LearnRate * 0.001f;
	cv::cuda::multiply(m_matFloatBgImgTemp, alpha, m_matFloatBgImgTemp);
	cv::cuda::multiply(m_matFloatBgImg, beta, m_matFloatBgImg);
	//step2
	cv::cuda::add(m_matFloatBgImgTemp, m_matFloatBgImg, m_matFloatBgImg);
	//step3
	m_matFloatBgImg.convertTo(m_matGrayBg, m_dst.type());
	// Subtract background
	if (m_bTrackDark){
		cv::cuda::subtract(m_matGrayBg, *m_src, m_dst);
	}
	else{
		cv::cuda::subtract(*m_src, m_matGrayBg, m_dst);
	}
	return true;
}
#endif

bool DynamicBGFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	} 
	if (floatBgImg.empty())
	{
		floatBgImg = cv::Mat::zeros(source.size(), CV_16UC1);
		floatBgImgTemp = cv::Mat::zeros(source.size(), CV_16UC1);
	}

	if ((int)(timeGetTime() - m_ExposureStartTime) > m_CameraExposureTime)
		m_bLearnBackground = true;

	//Capture full background
	if (m_bLearnBackground)
	{
		source.convertTo(floatBgImg, CV_16UC1, 65535.0f / 255.0f, 0);

		if (grayBg.empty())
		{
			grayBg = cv::Mat::zeros(source.size(), CV_8UC1);
		}
		//cvCopy(source, grayBg);
		m_ExposureStartTime = timeGetTime();
		m_bLearnBackground = false;
	}
	if (grayBg.empty())
		return false;

	//step1
	source.convertTo(floatBgImgTemp, CV_16UC1, 65535.0f / 255.0f, 0);
	//step2
	float fLearnRate = (float)m_LearnRate * 0.001f;
	cv::addWeighted(floatBgImgTemp, fLearnRate, floatBgImg, 1.0f - fLearnRate, 0, floatBgImg);
	//step3
	floatBgImg.convertTo(grayBg, CV_8UC1, 255.0f / 65535.0f, 0);

	//// Subtract background
	if (m_bTrackDark)
		cv::subtract(grayBg, source, destination);
	else
		cv::subtract(source, grayBg, destination);
	//IplImage* destination1 = cvCreateImage(cvSize(source->width, source->height), IPL_DEPTH_8U, 1);
	//cvSub(grayBg, source, destination1);
	////else
	//cvSub(source, grayBg, destination);
	//cvOr(destination, destination1, destination);
	return true;
}

void DynamicBGFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Learn background"), &m_bLearnBackground); 
	AddParameterBoolInput(_("Track dark blobs"), &m_bTrackDark); 
	AddParameterSpinInput(_("Background learn rate:"), &m_LearnRate, 1000, 0); 
	AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

wxFORCE_LINK_THIS_MODULE(DynamicBGFilter);