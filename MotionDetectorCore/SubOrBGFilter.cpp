#include "stdwx.h"
#include "SubOrBGFilter.h"
#include <opencv2/opencv.hpp>

#if defined(__LINUX__)
#include "LinuxUtils.h"
#endif
// LINUX_

IMPLEMENT_DYNAMIC_CLASS(SubOrBGFilter, Filter);

SubOrBGFilter::SubOrBGFilter()
{
	m_ExposureStartTime = timeGetTime();
	
	m_bLearnBackground = false;
	m_bStart = false;
	m_CameraExposureTime = 2200;
	m_bCalibrated = false;
	XS_SERIALIZE(m_CameraExposureTime, wxT("CameraExposureTime"));
	XS_SERIALIZE(m_bCalibrated, wxT("Calibrated"));
	XS_SERIALIZE(m_filename, wxT("filename"));
}

SubOrBGFilter::~SubOrBGFilter()
{

}

wxString SubOrBGFilter::GetName() const
{
	return _("Substruct OR BG");
}

#if defined(HAVE_CUDA)
bool SubOrBGFilter::KernelGPU()
{
	if (!m_bStart){
		int curTime = timeGetTime();
		if ((int)(curTime - m_ExposureStartTime) > m_CameraExposureTime){
			m_bLearnBackground = true;
			m_bStart = true;
		}
		else
			return false;
	}
	//Capture full background
	if (m_bLearnBackground)
	{
		m_src->copyTo(m_matBg);
		m_bLearnBackground = false;
	}
	// Subtract background
	if (m_bTrackDark){
		cv::cuda::subtract(m_matBg, *m_src, m_dst);
	}
	else{
		cv::cuda::subtract(*m_src, m_matBg, m_dst);
	}

	return true;
}
#endif


bool SubOrBGFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
	{
		return false;
	}

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}
	if (!m_bStart)
	{
		int curTime = timeGetTime();
		//wxLogDebug(wxT("%i, %i     %i > %i"), curTime, m_ExposureStartTime, curTime - m_ExposureStartTime, m_CameraExposureTime);
		if ((int)(curTime - m_ExposureStartTime) > m_CameraExposureTime)
		{
			m_bLearnBackground = true;
			m_bStart = true;
		}
		else
		{
			return false;
		}
	}
	//Capture full background
	if (m_bLearnBackground)
	{
		if (m_bCalibrated && wxFile::Exists(m_filename))
		{
			background = cv::imread("c:\\background.jpg", cv::IMREAD_GRAYSCALE);
		}
		else
		{
			if (background.empty())
			{
				background = cv::Mat::zeros(source.size(), CV_8UC1);
			}
			if (destination1.empty())
			{
				destination1 = cv::Mat::zeros(source.size(), CV_8UC1);
			}
			source.copyTo(background);
			//ResetCalibration();
		}
		m_bLearnBackground = false;
	}
	if (background.empty())
	{
		return false;
	}

	cv::subtract(background, source, destination1);
	cv::subtract(source, background, destination);
	cv::bitwise_or(destination, destination1, destination);
	return true;
}

void SubOrBGFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Learn background"), &m_bLearnBackground);
	AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

void SubOrBGFilter::SaveBackground(wxString filename)
{
	m_bCalibrated = !background.empty();
	if (m_bCalibrated)
	{
		m_filename = filename;
		//		cvSaveImage(/*filename.c_str()*/"c:\\background.jpg", background);
	}
}

void SubOrBGFilter::ResetCalibration()
{
	m_bCalibrated = false;
	m_bLearnBackground = true;
}

wxFORCE_LINK_THIS_MODULE(SubOrBGFilter);
//cvSub(background, source, destination1);
////else
//cvSub(source, background, destination);
//cvOr(destination, destination1, destination);
