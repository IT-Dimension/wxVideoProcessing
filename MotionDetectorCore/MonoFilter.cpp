#include "stdwx.h"
#include "MonoFilter.h"


IMPLEMENT_DYNAMIC_CLASS(MonoFilter, Filter);

MonoFilter::MonoFilter()
{
}

MonoFilter::~MonoFilter()
{
}

wxString MonoFilter::GetName() const
{
	return _("Mono");
}

#if defined(HAVE_CUDA)
bool MonoFilter::KernelGPU()
{
	if(!m_src)
		return false;
	//if(!m_dst) m_dst = new cv::gpu::GpuMat(m_src->size(), CV_8UC1);
	if (m_src->channels() == 4)
		cv::cuda::cvtColor(*m_src, m_dst, cv::COLOR_BGRA2GRAY);
	else if (m_src->channels() == 3)
		cv::cuda::cvtColor(*m_src, m_dst, cv::COLOR_BGR2GRAY);
	else
		return false;
	return true;
}
#endif

bool MonoFilter::Kernel()
{
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}


	if (source.channels() == 4)
	{
		cv::cvtColor(source, destination, cv::COLOR_BGRA2GRAY);
	}
	else if (source.channels() == 3)
	{
		cv::cvtColor(source, destination, cv::COLOR_BGR2GRAY);
	}
	else if (source.channels() == 1)
	{
		source.copyTo(destination);
	}
	else
	{
		return false;
	}

	return true;
}

wxFORCE_LINK_THIS_MODULE(MonoFilter);
