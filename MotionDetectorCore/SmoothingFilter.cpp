#include "stdwx.h"
#include "SmoothingFilter.h"

IMPLEMENT_DYNAMIC_CLASS(SmoothingFilter, Filter);

SmoothingFilter::SmoothingFilter()
{
	level = 0;
	XS_SERIALIZE(level, "Level");

}

SmoothingFilter::~SmoothingFilter()
{
}

wxString SmoothingFilter::GetName() const
{
	return _("Smoothing");
}

#if defined(HAVE_CUDA)
bool SmoothingFilter::KernelGPU()
{
	if (m_src->empty())
		return false;

	int lvl = level * 2 + 1;
	if (lvl > 1)
	{
		cv::cuda::blur(*m_src, m_dst, cv::Size(lvl, lvl));
	}
	else
	{
		m_src->copyTo(m_dst);
	}
	return true;
}
#endif

// The smooth filter really needs the blur size as a param
bool SmoothingFilter::Kernel()
{
	if (source.empty())
		return false;

	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}

	int lvl = level * 2 + 1;

	if (lvl > 1)
	{
		cv::blur(source, destination, cv::Size(lvl, lvl));
	}
	else
	{
		source.copyTo(destination);
	}
}

void SmoothingFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Smooth level:"), &level, 15);
}

wxFORCE_LINK_THIS_MODULE(SmoothingFilter);