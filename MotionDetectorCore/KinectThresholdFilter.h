#ifndef __TOUCHSCREEN_FILTER_KINECTTHRESHOLD__
#define __TOUCHSCREEN_FILTER_KINECTTHRESHOLD__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>
#include <mmsystem.h>

// This filter has two modes. One that dynamically changes the threshold with the help
// of statistics that are recorded, and one which is static. Idea by Damian.
class MOTION_DETECTOR_CORE_API KinectThresholdFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(KinectThresholdFilter)
public:
	KinectThresholdFilter();
	virtual ~KinectThresholdFilter();
	
	bool Kernel();
	virtual wxString GetName() const;

private:

	int thresholdSlider1;
	int thresholdSlider2;
	bool m_bLearnBackground;
	int m_ExposureStartTime;
	int m_CameraExposureTime;
	int m_LearnRate;

protected:
	void CreateParamInputs();
};

#endif // __TOUCHSCREEN_FILTER_KINECTTHRESHOLD__
