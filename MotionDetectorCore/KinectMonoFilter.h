#ifndef __TOUCHLIB_FILTER_KINECTMONO__
#define __TOUCHLIB_FILTER_KINECTMONO__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API KinectMonoFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(KinectMonoFilter)
public:
	KinectMonoFilter();
	virtual ~KinectMonoFilter();

	bool Kernel();
	virtual wxString GetName() const;
	// This filter cannot be disabled in settings
	virtual bool IsMandatory() { return true; }

private:

};

#endif // __TOUCHLIB_FILTER_KINECTMONO__
