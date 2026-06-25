#ifndef __TOUCHLIB_FILTER_AMPLIFY__
#define __TOUCHLIB_FILTER_AMPLIFY__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API AmplifyFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(AmplifyFilter)
public:
	AmplifyFilter();
	virtual ~AmplifyFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
	cv::cuda::GpuMat m_floatGpuMat;
#endif
	virtual wxString GetName() const;

	int level;

protected:
	virtual void CreateParamInputs();

};

#endif // __TOUCHLIB_FILTER_AMPLIFY__
