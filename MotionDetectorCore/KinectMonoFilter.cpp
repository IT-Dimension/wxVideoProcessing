#include "stdwx.h"
#include "KinectMonoFilter.h"

#if defined(__linux__)
typedef unsigned short WORD;
#endif

IMPLEMENT_DYNAMIC_CLASS(KinectMonoFilter, Filter);

KinectMonoFilter::KinectMonoFilter()
{
}

KinectMonoFilter::~KinectMonoFilter()
{
}

wxString KinectMonoFilter::GetName() const
{
	return _("Mono");
}

bool KinectMonoFilter::Kernel()
{
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_32FC1);
	}

	if (source.channels() == 3)
	{
		int rows = source.rows;
		int cols = source.cols;

		for (int y = 0; y < rows; ++y)
		{
			const unsigned char* srcRow = source.ptr<unsigned char>(y);
			float* dstRow = destination.ptr<float>(y);

			for (int x = 0; x < cols; ++x)
			{
				const unsigned char* dataPtr = srcRow + (x * 3) + 1;

				WORD depthValue = *reinterpret_cast<const WORD*>(dataPtr);
				dstRow[x] = static_cast<float>(depthValue) / 4095.0f;
			}
		}
	}
	else if (source.channels() == 1)
	{
	
		source.convertTo(destination, CV_32F, 1.0 / 4095.0);
	}
	else
	{
		return false;
	}
	//if (source->nChannels != 1 && destination->nChannels == 1) 
	//{
	//	if (strcmpi(source->colorModel, "BGRA") == 0)
	//		cvCvtColor(source, destination, CV_BGRA2GRAY);
	//	else if (strcmpi(source->colorModel, "BGR") == 0)
	//		cvCvtColor(source, destination, CV_BGR2GRAY);
	//	else if (strcmpi(source->colorModel, "RGB") == 0) 
	//		cvCvtColor(source, destination, CV_RGB2GRAY);
	//	else
	//		return false;
	//}
	//else
	//	return false;
	return true;
}

wxFORCE_LINK_THIS_MODULE(KinectMonoFilter);