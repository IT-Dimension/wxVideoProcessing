#ifndef __TOUCHLIB_FILTER_FLIP__
#define __TOUCHLIB_FILTER_FLIP__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class IFloorCameraSettings;

class MOTION_DETECTOR_CORE_API FlipFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(FlipFilter)
public:
	FlipFilter();
	virtual ~FlipFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;
	virtual bool IsEnabled();

	void SetSettings(const IFloorCameraSettings & settings);

	bool bFlipVertically;
	bool bFlipHorizontally;
protected:
	void CreateParamInputs();
private:

};

#endif // __TOUCHLIB_FILTER_FLIP__
