#ifndef __TOUCHSCREEN_FILTER_SMOOTHING__
#define __TOUCHSCREEN_FILTER_SMOOTHING__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API SmoothingFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(SmoothingFilter)
public:
	SmoothingFilter();
	virtual ~SmoothingFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;

	int level;
protected:
	void CreateParamInputs();
};

#endif // __TOUCHSCREEN_FILTER_SMOOTHING__
