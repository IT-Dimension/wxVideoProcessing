#include "stdwx.h"
#include "KinectBGFilter.h"
//#include <highgui.h>
#include <opencv2/highgui/highgui_c.h>
#if defined(__linux__)
#include "LinuxUtils.h"
#endif
// LINUX_

IMPLEMENT_DYNAMIC_CLASS(KinectBGFilter, Filter);

KinectBGFilter::KinectBGFilter()
{
	m_bTrackDark = false;
	m_ExposureStartTime = timeGetTime();
	m_bLearnBackground = false;
	m_bStart = false;
	m_CameraExposureTime = 2200;
	m_bCalibrated = false;
	XS_SERIALIZE(m_bTrackDark, wxT("TrackDark"));
	XS_SERIALIZE(m_CameraExposureTime, wxT("CameraExposureTime"));
	XS_SERIALIZE(m_bCalibrated, wxT("Calibrated"));
	XS_SERIALIZE(m_filename, wxT("filename"));
}

KinectBGFilter::~KinectBGFilter()
{

}

wxString KinectBGFilter::GetName() const
{
	return _("Simple BG");
}

#if defined(HAVE_CUDA)
bool KinectBGFilter::KernelGPU()
{
	if (!m_bStart){ 
		int curTime = timeGetTime();
		if((int)(curTime - m_ExposureStartTime) > m_CameraExposureTime){
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


bool KinectBGFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	} 
	if (!m_bStart){ 
		int curTime = timeGetTime();
		if((int)(curTime - m_ExposureStartTime) > m_CameraExposureTime){
			m_bLearnBackground = true;
			m_bStart = true;
		}
		else
			return false;
	}
	//Capture full background
	if (m_bLearnBackground)
	{
		if(m_bCalibrated && wxFile::Exists(m_filename)){
			
			background = cv::imread("c:\\background.jpg", cv::IMREAD_UNCHANGED);
		}
		else{
			if (background.empty())
			{
				background = cv::Mat::zeros(source.size(), source.type());
			}
			source.copyTo(background);
			//ResetCalibration();
		}
		m_bLearnBackground = false;
	}
	if (background.empty())
		return false;

	// Subtract background
	if (m_bTrackDark)
		cv::subtract(background, source, destination);
	else
		cv::subtract(source, background, destination);

	return true;
}

void KinectBGFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Learn background"), &m_bLearnBackground); 
	AddParameterBoolInput(_("Track dark blobs"), &m_bTrackDark);
	AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

void KinectBGFilter::SaveBackground(wxString filename)
{
	m_bCalibrated = !background.empty();
	if(m_bCalibrated)
	{
		m_filename = filename;
		cv::imwrite("c:\\background.jpg", background);
	}
}

void KinectBGFilter::ResetCalibration()
{
	m_bCalibrated = false;
	m_bLearnBackground = true;
}

wxFORCE_LINK_THIS_MODULE(KinectBGFilter);