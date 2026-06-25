#ifndef _MOTIONDETECTORMULTICAMPLUGIN_H
#define _MOTIONDETECTORMULTICAMPLUGIN_H

#include <MotionDetectorPluginBase.h>

/// Base class for all iFloor effect plugins
class MotionDetectorMultiCamPlugin : public MotionDetectorPluginBase
{
	DECLARE_DYNAMIC_CLASS(MotionDetectorMultiCamPlugin)
public:
	MotionDetectorMultiCamPlugin();
	~MotionDetectorMultiCamPlugin();
	virtual wxString GetID() const;
	virtual wxString GetName() const;
	
	virtual MotionDetectorBase * CreateDetector(GeometryProvider && geometryProvider);
private:
	wxString m_ID;
};

#endif // _MOTIONDETECTORMULTICAMPLUGIN_H