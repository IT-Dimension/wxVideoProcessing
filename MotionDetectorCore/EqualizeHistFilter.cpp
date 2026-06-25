#include "stdwx.h"
#include "EqualizeHistFilter.h"

IMPLEMENT_DYNAMIC_CLASS(EqualizeHistFilter, Filter);

EqualizeHistFilter::EqualizeHistFilter()
{
	level = 0;
}

EqualizeHistFilter::~EqualizeHistFilter()
{

}

wxString EqualizeHistFilter::GetName() const
{
	return _("Equalize Histogram");
}

#if defined(HAVE_CUDA)
bool EqualizeHistFilter::KernelGPU()
{
	return false;
}
#endif

bool EqualizeHistFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());

		if (source.channels() == 3)
		{
			cv::Mat hsv;
			cv::cvtColor(source, hsv, cv::COLOR_BGR2HSV);
			std::vector<cv::Mat> hsvChannels;
			cv::split(hsv, hsvChannels);
			cv::merge(hsvChannels, hsv);
			cv::cvtColor(hsv, destination, cv::COLOR_HSV2BGR);
		}
	}

	else if (source.channels() == 1)
	{
		cv::equalizeHist(source, destination);
	}

	return true;
}

void EqualizeHistFilter::CreateParamInputs()
{
}

wxFORCE_LINK_THIS_MODULE(EqualizeHistFilter);