#ifndef __TOUCHSCREEN_FILTER_SIMPLE_HIGHPASS__
#define __TOUCHSCREEN_FILTER_SIMPLE_HIGHPASS__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

// This filter is used for filtering out sharp-edged objects from an image.
class MOTION_DETECTOR_CORE_API SimpleHighpassFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(SimpleHighpassFilter)
public:
	SimpleHighpassFilter();
	virtual ~SimpleHighpassFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;

private:

	// ----  constants  ----------------------------------------------------------

	static const int DEFAULT_BLUR_LEVEL		= 10;
	static const int DEFAULT_NOISE_METHOD	= 1;
	static const int DEFAULT_NOISE_LEVEL	= 3;

	static const int NOISE_METHOD_MEDIAN	= 0;
	static const int NOISE_METHOD_BLUR		= 1;

	// ----  instance variables  -------------------------------------------------

	int blurLevel;
	int noiseSmoothType;
	int noiseLevel;

	cv::Mat buffer;
#if defined(HAVE_CUDA)
	cv::cuda::GpuMat m_matBg;
#endif

	// ----  methods  ------------------------------------------------------------

	void setNoiseSmoothType(int noiseMethod);
protected:
	void CreateParamInputs();
};

#endif // __TOUCHSCREEN_FILTER_SIMPLE_HIGHPASS__
