#ifndef __TOUCHSCREEN_FILTER_RECTIFY__
#define __TOUCHSCREEN_FILTER_RECTIFY__

#include "FilterTemplate.h"

#define DEFAULT_RECTIFYLEVEL 20

class MOTION_DETECTOR_CORE_API RectifyFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(RectifyFilter)
public:
	RectifyFilter();
	virtual ~RectifyFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

	void setLevel(unsigned int value) {level = value; level_slider = level;}
	unsigned int getLevel(void) {return level;}
protected:
	void CreateParamInputs();

private:
	bool bAutoSet;
	int level_slider;
	int level;
};

#endif // __TOUCHSCREEN_FILTER_RECTIFY__
