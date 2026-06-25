#include "stdwx.h"
#include "KinectDepthBGFilter.h"

#if defined(__linux__)
#include "LinuxUtils.h"
typedef unsigned short WORD;
#endif

IMPLEMENT_DYNAMIC_CLASS(KinectDepthBGFilter, Filter);

KinectDepthBGFilter::KinectDepthBGFilter()
{
	m_bStart = false;
	m_bLearnBackground = false;
	m_ExposureStartTime = timeGetTime();
	m_CameraExposureTime = 2200;
	XS_SERIALIZE(m_CameraExposureTime, wxT("CameraExposureTime"));
}

KinectDepthBGFilter::~KinectDepthBGFilter()
{
}

wxString KinectDepthBGFilter::GetName() const
{
	return _("Depth BG");
}

bool KinectDepthBGFilter::Kernel()
{
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
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
		if (background.empty())
		{
			background = cv::Mat::zeros(source.size(), source.type());
		}
		source.copyTo(background);

		m_bLearnBackground = false;
	}
	if (background.empty())
		return false;

	int rows = source.rows;
	int cols = source.cols;

	for (int y = 0; y < rows; y++)
	{
		const float* pSrc = source.ptr<float>(y);
		const float* pBg = background.ptr<float>(y);
		float* pDst = destination.ptr<float>(y);

		for (int x = 0; x < cols; x++)
		{
			if (pSrc[x] == 0.0f || pBg[x] == 0.0f || pSrc[x] >= pBg[x])
			{
				pDst[x] = 0.0f;
			}
			else
			{
				pDst[x] = pBg[x] - pSrc[x];
			}
		}
	}
	return true;

}
void KinectDepthBGFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Learn background"), &m_bLearnBackground);
	AddParameterSpinInput(_("Camera exposure time:"), &m_CameraExposureTime, 20000, 0);
}

wxFORCE_LINK_THIS_MODULE(KinectDepthBGFilter);