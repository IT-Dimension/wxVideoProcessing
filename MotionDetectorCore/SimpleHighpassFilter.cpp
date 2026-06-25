#include "stdwx.h"
#include "SimpleHighpassFilter.h"

IMPLEMENT_DYNAMIC_CLASS(SimpleHighpassFilter, Filter);

SimpleHighpassFilter::SimpleHighpassFilter()
{
	noiseSmoothType = 1;
	blurLevel = DEFAULT_BLUR_LEVEL;

	setNoiseSmoothType(DEFAULT_NOISE_METHOD);

	noiseLevel = DEFAULT_NOISE_LEVEL;

	XS_SERIALIZE(blurLevel, wxT("BlurLevel"));
	XS_SERIALIZE(noiseLevel, wxT("NoiseLevel"));
	XS_SERIALIZE(noiseSmoothType, wxT("NoiseSmoothType"));
}

SimpleHighpassFilter::~SimpleHighpassFilter()
{

}

wxString SimpleHighpassFilter::GetName() const
{
	return _("Simple Highpass");
}

void SimpleHighpassFilter::setNoiseSmoothType(int noiseMethod)
{
	//switch (noiseMethod)
	//{
	//case NOISE_METHOD_MEDIAN:
	//	noiseSmoothType = CV_MEDIAN;
	//	break;
	//case NOISE_METHOD_BLUR:
		noiseSmoothType = 1;
	//	break;
	//}
}
#if defined(HAVE_CUDA)
bool SimpleHighpassFilter::KernelGPU()
{
	int blurParameter = blurLevel * 2 + 1;
	int noiseParameter = noiseLevel * 2 + 1;
	cv::Mat tmp, tmpDst;
	if(blurLevel){
		cv::cuda::blur(*m_src, m_tmpGpuMat, cv::Size(blurParameter, blurParameter));
		cv::cuda::subtract(*m_src, m_tmpGpuMat, m_tmpGpuMat);
		m_tmpGpuMat.download(tmp);
	}
	else
		m_src->download(tmp);
	
	cv::medianBlur(tmp, tmpDst, noiseParameter);
	m_dst.upload(tmpDst);
	return true;
}
#endif


bool SimpleHighpassFilter::Kernel()
{
	if (source.empty())
	{
		return false;
	}


	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	if (buffer.empty())
	{
		buffer = cv::Mat::zeros(source.size(), source.type());
	}

	int blurParameter = blurLevel * 2 + 1;
	int noiseParameter = noiseLevel * 2 + 1;
	// create the unsharp mask using a linear average filter
	cv::blur(source, buffer, cv::Size(blurParameter, blurParameter));
	//cvAbsDiff(source, buffer, buffer);
	cv::subtract(source, buffer, buffer);

	// filter out the noise using a median filter
	cv::medianBlur(buffer, destination, noiseParameter);
	return true;
}

void SimpleHighpassFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Blur level:"), &blurLevel, 200, 0);
	AddParameterSpinInput(_("Noise level:"), &noiseLevel, 30, 0);
}

wxFORCE_LINK_THIS_MODULE(SimpleHighpassFilter);
