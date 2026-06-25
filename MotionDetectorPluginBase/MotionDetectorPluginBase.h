#ifndef _MOTIONDETECTORPLUGINBASE_H
#define _MOTIONDETECTORPLUGINBASE_H

#include "MotionDetectorPlugin.h"

class MotionDetectorBase;
class GeometryProvider;

/// Base class for all iFloor effect plugins
class IFLOOR_API MotionDetectorPluginBase : public wxObject
{
	DECLARE_ABSTRACT_CLASS(MotionDetectorPluginBase)
public:
	virtual ~MotionDetectorPluginBase();
	/// Returns GUID (unique identifier) of plugin
	/// \return string which contains unique identifier of plugin
	virtual wxString GetID() const = 0;
	/// Returns name of plugin
	/// \return string which contains human-readable name of plugin
	virtual wxString GetName() const = 0;
	virtual MotionDetectorBase * CreateDetector(GeometryProvider && geometryProvider) = 0;
	virtual void DeleteDetector(MotionDetectorBase * detector);
};

typedef MotionDetectorPluginBase * (*CreateMotionDetectorPlugin_function)();
typedef void (*DeleteMotionDetectorPlugin_function)(MotionDetectorPluginBase * plugin);

#endif // _MOTIONDETECTORPLUGINBASE_H