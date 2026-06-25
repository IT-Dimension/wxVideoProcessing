#ifndef __TOUCHLIB_FILTER_EQ_HIST__
#define __TOUCHLIB_FILTER_EQ_HIST__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API EqualizeHistFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(EqualizeHistFilter)
public:
	EqualizeHistFilter();
	virtual ~EqualizeHistFilter();
	
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

#endif // __TOUCHLIB_FILTER_EQ_HIST__
