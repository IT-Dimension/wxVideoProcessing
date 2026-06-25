#ifndef __TOUCHSCREEN_FILTER_HIGHPASS__
#define __TOUCHSCREEN_FILTER_HIGHPASS__

#include "FilterTemplate.h"

class MOTION_DETECTOR_CORE_API HighpassFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(HighpassFilter)
public:
	HighpassFilter();
	virtual ~HighpassFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

protected:
	void CreateParamInputs();

private:
	int filterLevel;
	int filterLevel_slider;

	int scale;
	int scale_slider;

	cv::Mat element;
	cv::Mat element2;

	bool bErodeDialate;
};

#endif // __TOUCHSCREEN_FILTER_HIGHPASS__
