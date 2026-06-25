//#include "stdwx.h"
#include "AmplifyFilter.h"

IMPLEMENT_DYNAMIC_CLASS(AmplifyFilter, Filter);

AmplifyFilter::AmplifyFilter()
{
	level = 0;
	XS_SERIALIZE(level, wxT("AmplifyLevel"));
}

AmplifyFilter::~AmplifyFilter()
{
}

wxString AmplifyFilter::GetName() const
{
	return _("Amplify");
}

#if defined(HAVE_CUDA)
bool AmplifyFilter::KernelGPU()
{
	if (m_src->empty())
		return false;

	double scalef = static_cast<double>(level) / 128.0;
	cv::cuda::multiply(*m_src, *m_src, m_dst, scalef);
	return true;
}
#endif

bool AmplifyFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}

	float scalef = level / 128.0f;

	cv::multiply(source, source, destination, scalef);
	return true;
}

void AmplifyFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Amplify level:"), &level, 300);
}

wxFORCE_LINK_THIS_MODULE(AmplifyFilter);