#ifndef __TOUCHLIB_FILTER_BRIGHTNESSCONTRAST__
#define __TOUCHLIB_FILTER_BRIGHTNESSCONTRAST__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

#define DEFAULT_BRIGHTNESS 0.0
#define DEFAULT_CONTRAST 0.2

class MOTION_DETECTOR_CORE_API BrightnessContrastFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(BrightnessContrastFilter)
public:
	BrightnessContrastFilter();
	virtual ~BrightnessContrastFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;

	void setBrightness(float value);
	void setContrast(float value);
	float getContrast(void) {return contrast;}
	float getBrightness(void) {return brightness;}

protected:
	void CreateParamInputs();
private:
	void updateLUT( void );

	uchar lut[256*4];

	int brightness_slider;
	int contrast_slider;

	float brightness;
	float contrast;
	cv::Mat lutMat;
};

#endif // __TOUCHLIB_FILTER_BRIGHTNESSCONTRAST__
