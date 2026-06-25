#ifndef __TOUCHSCREEN_FILTER_THRESHOLD__
#define __TOUCHSCREEN_FILTER_THRESHOLD__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

// This filter has two modes. One that dynamically changes the threshold with the help
// of statistics that are recorded, and one which is static. Idea by Damian.
class MOTION_DETECTOR_CORE_API ThresholdFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(ThresholdFilter)
public:
	ThresholdFilter();
	virtual ~ThresholdFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;

private:

	int thresholdSlider1;

protected:
	void CreateParamInputs();
};

#endif // __TOUCHSCREEN_FILTER_THRESHOLD__
