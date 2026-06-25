#ifndef __TOUCHLIB_FILTER_RESIZE__
#define __TOUCHLIB_FILTER_RESIZE__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

#define DEFAULT_RESIZEWIDTH 480
#define DEFAULT_RESIZEHEIGHT 640

class MOTION_DETECTOR_CORE_API ResizeFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(ResizeFilter)
public:
	ResizeFilter();
	virtual ~ResizeFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

private:

	int sizeX;
	int sizeY;
};

#endif // __TOUCHLIB_FILTER_RESIZE__
