#ifndef _VIDEOSOURCEDIRECTSHOWPLUGIN_H
#define _VIDEOSOURCEDIRECTSHOWPLUGIN_H

#include <VideoSourcePluginBase.h>

class CDeviceManager;

/// Base class for all iFloor effect plugins
class VideoSourceDirectShowPlugin : public VideoSourcePluginBase
{
	DECLARE_DYNAMIC_CLASS(VideoSourceDirectShowPlugin)
public:
	VideoSourceDirectShowPlugin();
	~VideoSourceDirectShowPlugin();
	virtual wxString GetID() const;
	virtual wxString GetName() const;

	virtual size_t GetDeviceCount() const;
	virtual wxString GetName(size_t index) const;
	virtual wxString GetName(const wxString & id) const;
	virtual wxString GetID(size_t index) const;
	virtual VideoGrabberBase * CreateGrabber(size_t index);
	virtual VideoGrabberBase * CreateGrabber(const wxString & id);
	virtual void DeleteGrabber(VideoGrabberBase * grabber);
	virtual void RefreshDevices();
private:
	wxString m_ID;
	CDeviceManager * m_pDeviceManager;
};

#endif // _VIDEOSOURCEDIRECTSHOWPLUGIN_H