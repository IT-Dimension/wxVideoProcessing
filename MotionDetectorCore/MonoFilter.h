#ifndef __TOUCHLIB_FILTER_MONO__
#define __TOUCHLIB_FILTER_MONO__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API MonoFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(MonoFilter)
public:
	MonoFilter();
	virtual ~MonoFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	virtual bool KernelGPU();
#endif
	virtual wxString GetName() const;
	// This filter cannot be disabled in settings
	virtual bool IsMandatory() { return true; }

private:

};

#endif // __TOUCHLIB_FILTER_MONO__
