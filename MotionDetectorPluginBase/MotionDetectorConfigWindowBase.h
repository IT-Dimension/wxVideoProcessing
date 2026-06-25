#ifndef _MOTIONDETECTORCONFIGWINDOWBASE_H
#define _MOTIONDETECTORCONFIGWINDOWBASE_H
#include "MotionDetectorPlugin.h"
#include <CommonConfigWindowBase.h>

class MotionDetectorBase;

class IFLOOR_API MotionDetectorConfigWindowBase : public CommonConfigWindowBase
{
	DECLARE_DYNAMIC_CLASS(MotionDetectorConfigWindowBase)
public:
	MotionDetectorConfigWindowBase();
	MotionDetectorConfigWindowBase(MotionDetectorBase * detector, wxWindow * parent);

	bool Create(MotionDetectorBase * detector, wxWindow * parent);
	virtual ~MotionDetectorConfigWindowBase(void);

	/// Reads config from the motion detector
	virtual bool ReadConfig();

	/// Saves config to the motion detector
	virtual bool SaveConfig();

	virtual MotionDetectorBase * GetDetector();
	virtual void StartDetection();
	virtual void StopDetection();

protected:
	MotionDetectorBase * m_Detector;
	bool m_Started;
};
#endif // _MOTIONDETECTORCONFIGWINDOWBASE_H
