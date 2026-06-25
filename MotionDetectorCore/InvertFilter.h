#ifndef __TOUCHSCREEN_FILTER_INVERT__
#define __TOUCHSCREEN_FILTER_INVERT__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API InvertFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(InvertFilter)
public:
	InvertFilter();
	virtual ~InvertFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

private:

};

#endif // __TOUCHSCREEN_FILTER_INVERT__
