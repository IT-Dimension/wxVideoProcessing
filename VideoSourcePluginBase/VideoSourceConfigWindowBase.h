#ifndef _VIDEOSOURCECONFIGWINDOWBASE_H
#define _VIDEOSOURCECONFIGWINDOWBASE_H

#include "VideoSourcePlugin.h"
#include <CommonConfigWindowBase.h>

class VideoSourcePluginBase;

class IFLOOR_API VideoSourceConfigWindowBase : public CommonConfigWindowBase
{
public:
	VideoSourceConfigWindowBase();
	VideoSourceConfigWindowBase(VideoSourcePluginBase * plugin, wxWindow * parent);

	bool Create(VideoSourcePluginBase * plugin, wxWindow * parent);
	virtual ~VideoSourceConfigWindowBase(void);

	/// Reads config from the video source plugin
	virtual bool ReadConfig();

	/// Saves config to the video source plugin
	virtual bool SaveConfig();

protected:
	VideoSourcePluginBase * m_Plugin;
};

#endif // _VIDEOSOURCECONFIGWINDOWBASE_H