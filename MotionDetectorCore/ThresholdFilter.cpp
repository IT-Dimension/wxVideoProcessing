#include "stdwx.h"
#include "ThresholdFilter.h"


// ----  initialization of non-integral constants  ----------------------------


// ----  implementations  -----------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(ThresholdFilter, Filter);

ThresholdFilter::ThresholdFilter()
{
	thresholdSlider1 = 1;

	XS_SERIALIZE(thresholdSlider1, wxT("Threshold1"));

}

ThresholdFilter::~ThresholdFilter()
{
}

wxString ThresholdFilter::GetName() const
{
	return _("Threshold");
}

#if defined(HAVE_CUDA)
bool ThresholdFilter::KernelGPU()
{
	cv::cuda::threshold(*m_src, m_dst, thresholdSlider1, 255, cv::THRESH_BINARY);
	return true;
}
#endif


bool ThresholdFilter::Kernel()
{
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	
	cv::threshold(source, destination, thresholdSlider1, 255, cv::THRESH_BINARY);

	return true;
}

void ThresholdFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Threshold level:"), &thresholdSlider1, 255);
}

wxFORCE_LINK_THIS_MODULE(ThresholdFilter);