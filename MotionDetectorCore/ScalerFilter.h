#ifndef __TOUCHSCREEN_FILTER_SCALER__
#define __TOUCHSCREEN_FILTER_SCALER__

#include "FilterTemplate.h"

#define DEFAULT_RECTIFYLEVEL 20

class MOTION_DETECTOR_CORE_API ScalerFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(ScalerFilter)
public:
	ScalerFilter();
	virtual ~ScalerFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

	void setLevel(unsigned int value) { level = value; }
	unsigned int getLevel(void) { return level; }

private:
	bool bAutoSet;
	unsigned int level;
};

#endif // __TOUCHSCREEN_FILTER_RECTIFY__
