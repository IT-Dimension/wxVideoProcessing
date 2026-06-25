#ifndef __TOUCHLIB_FILTER_KINECTDEPTHBG__
#define __TOUCHLIB_FILTER_KINECTDEPTHBG__

#include "FilterTemplate.h"
#include <mmsystem.h>
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API KinectDepthBGFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(KinectDepthBGFilter)
public:
	KinectDepthBGFilter();
	virtual ~KinectDepthBGFilter();

	bool Kernel();
	virtual wxString GetName() const;
	// This filter cannot be disabled in settings
	//virtual bool IsMandatory() { return true; }
protected:
	void CreateParamInputs();

private:
	cv::Mat background;
	bool m_bLearnBackground;
	int m_ExposureStartTime;
	int m_CameraExposureTime;
	bool m_bStart;
};

#endif // __TOUCHLIB_FILTER_KinectDepthBGFilter__
