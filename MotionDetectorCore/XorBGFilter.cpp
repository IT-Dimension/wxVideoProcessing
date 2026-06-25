#include "stdwx.h"
#include "XorBGFilter.h"
//#include <highgui.h>
#include <opencv2/highgui/highgui_c.h>

#if defined(__LINUX__)
#include "LinuxUtils.h"
#endif
// LINUX_

IMPLEMENT_DYNAMIC_CLASS(XorBGFilter, Filter);

XorBGFilter::XorBGFilter()
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

XorBGFilter::~XorBGFilter()
{
}

wxString XorBGFilter::GetName() const
{
	return _("XOR Background");
}

#if defined(HAVE_CUDA)
bool XorBGFilter::KernelGPU()
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


bool XorBGFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
	{
		return false;
	}

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
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
			background = cv::imread("c:\\background.jpg", cv::IMREAD_UNCHANGED);
		}
		else
		{
			if (background.empty())
			{
				background = cv::Mat::zeros(source.size(), source.type());
			}

			// Копіюємо вміст через сучасний C++ метод
			source.copyTo(background);
		}
		m_bLearnBackground = false;
	}

	if (background.empty())
	{
		return false;
	}

	cv::bitwise_xor(background, source, destination);
	return true;
}

void XorBGFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Learn background"), &m_bLearnBackground);
	AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

void XorBGFilter::SaveBackground(wxString filename)
{
	m_bCalibrated = !background.empty();
	if (m_bCalibrated)
	{
		m_filename = filename;
		cv::imwrite("c:\\background.jpg", background);
	}
}

void XorBGFilter::ResetCalibration()
{
	m_bCalibrated = false;
	m_bLearnBackground = true;
}

wxFORCE_LINK_THIS_MODULE(XorBGFilter);
//cvSub(background, source, destination1);
////else
//cvSub(source, background, destination);
//cvOr(destination, destination1, destination);
