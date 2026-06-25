#include "stdwx.h"
#include "KinectThresholdFilter.h"

#if defined(__linux__)
#include "LinuxUtils.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(KinectThresholdFilter, Filter);

KinectThresholdFilter::KinectThresholdFilter()
{
	thresholdSlider1 = 0;
	thresholdSlider2 = 4095;
	m_bLearnBackground = false;
	m_LearnRate = 400;
	m_CameraExposureTime = 2200;
#if defined(HAVE_CUDA)
	bStart = false;
#endif

	m_ExposureStartTime = timeGetTime();
	XS_SERIALIZE(m_LearnRate, wxT("LearnRate"));
	XS_SERIALIZE(m_bLearnBackground, wxT("LearnBackground"));
	XS_SERIALIZE(m_CameraExposureTime, wxT("CameraExposureTime"));
	XS_SERIALIZE(thresholdSlider1, wxT("Threshold1"));
	XS_SERIALIZE(thresholdSlider2, wxT("Threshold2"));

}

KinectThresholdFilter::~KinectThresholdFilter()
{
}

wxString KinectThresholdFilter::GetName() const
{
	return _("Threshold & Dynamic BG");
}

bool KinectThresholdFilter::Kernel()
{
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}
	//if (!floatBgImg)
	//{
	//	floatBgImg = cvCreateImage(cvSize(source->width, source->height), source->depth, source->nChannels);
	//	floatBgImgTemp = cvCreateImage(cvSize(source->width, source->height), source->depth, source->nChannels);
	//}

	//if ((int)(timeGetTime() - m_ExposureStartTime) > m_CameraExposureTime)
	//	m_bLearnBackground = true;

	////Capture full background
	//if (m_bLearnBackground)
	//{
	//	//cvConvertScale(source, floatBgImg, 65535.0f / 255.0f, 0);
	//	cvCopy(source, floatBgImg);
	//	if (!grayBg)
	//	{
	//		grayBg = cvCreateImage(cvSize(source->width, source->height), 8, 1);
	//	}
	//	m_ExposureStartTime = timeGetTime();
	//	m_bLearnBackground = false;
	//}
	//if (grayBg == NULL)
	//	return false;

	//float fLearnRate = (float)m_LearnRate * 0.001f;
	//cvAddWeighted(source, fLearnRate, floatBgImg, 1.0f - fLearnRate, 0, floatBgImg);
	//cvSub(source, floatBgImg, floatBgImg);
	//cvConvertScale(floatBgImg, grayBg, 128.0f, 0);

	float minLevel = static_cast<float>(std::min(thresholdSlider1, thresholdSlider2)) / 4095.0f;
	float maxLevel = static_cast<float>(std::max(thresholdSlider1, thresholdSlider2)) / 4095.0f;

	cv::inRange(source, cv::Scalar(minLevel), cv::Scalar(maxLevel), destination);
	return true;
}

void KinectThresholdFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Threshold level 1:"), &thresholdSlider1, 0xfff);
	AddParameterSpinInput(_("Threshold level 2:"), &thresholdSlider2, 0xfff);
	//AddParameterBoolInput(_("Learn background"), &m_bLearnBackground);
	//AddParameterSpinInput(_("Background learn rate:"), &m_LearnRate, 1000, 0);
	//AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

wxFORCE_LINK_THIS_MODULE(KinectThresholdFilter);