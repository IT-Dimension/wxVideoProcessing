#ifndef __TOUCHSCREEN_FILTER_SHAPE__
#define __TOUCHSCREEN_FILTER_SHAPE__

#include "FilterTemplate.h"

class MOTION_DETECTOR_CORE_API ShapeFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(ShapeFilter)
public:
	ShapeFilter();
	virtual ~ShapeFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

private:
	static const int DEFAULT_BLUR_LEVEL		= 10;
	static const char *TRACKBAR_LABEL_BLUR;
	static const char *PARAMETER_BLUR;

	static const int DEFAULT_LEVEL_LEVEL		= 0;
	static const char *TRACKBAR_LABEL_LEVEL;
	static const char *PARAMETER_LEVEL;

	int blurLevel;
	int levelLevel;

	cv::Mat buffer;

	void setNoiseSmoothType(int noiseMethod);
};

#endif // __TOUCHSCREEN_FILTER_SIMPLE_HIGHPASS__
