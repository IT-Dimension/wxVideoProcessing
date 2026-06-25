#include "stdwx.h"
#include <VideoSourcePluginBase.h>
#include "VideoSourceDirectShowPlugin.h"

WXEXPORT VideoSourcePluginBase * CreatePlugin()
{
	return new VideoSourceDirectShowPlugin;
}

WXEXPORT void DeletePlugin(VideoSourcePluginBase * plugin)
{
	wxDELETE(plugin);
}
