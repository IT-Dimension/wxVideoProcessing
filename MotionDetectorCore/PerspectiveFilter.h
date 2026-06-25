#ifndef __TOUCHSCREEN_FILTER_PERSPECTIVE__
#define __TOUCHSCREEN_FILTER_PERSPECTIVE__

#include "FilterTemplate.h"

class IFloorCameraSettings;

class MOTION_DETECTOR_CORE_API PerspectiveFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(PerspectiveFilter)
public:
	PerspectiveFilter();
	virtual ~PerspectiveFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
	cv::Mat src;
	cv::Mat dst;

#endif
	virtual wxString GetName() const;

	void SetSettings(const IFloorCameraSettings & settings);

private:
	std::vector<cv::Point2f> cvsrc;
	std::vector<cv::Point2f> cvdst;
	cv::Mat _translate;
	bool Translate();
	float xMax, xMin, yMin, yMax;

};

#endif // __TOUCHSCREEN_FILTER_INVERT__
