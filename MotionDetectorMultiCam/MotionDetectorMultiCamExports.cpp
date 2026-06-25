#include "stdwx.h"
#include <MotionDetectorPluginBase.h>
#include "MotionDetectorMultiCamPlugin.h"
#include "MotionDetectorMultiCamGuiPlugin.h"

PLUGIN_EXPORTED_API MotionDetectorPluginBase * CreatePlugin()
{
	return new MotionDetectorMultiCamPlugin;
}

PLUGIN_EXPORTED_API void DeletePlugin(MotionDetectorPluginBase * plugin)
{
	wxDELETE(plugin);
}
