#ifndef _VIDEOSOURCEPLUGINBASE_H
#define _VIDEOSOURCEPLUGINBASE_H

#include <SerializableBase.h>
#include "VideoSourcePlugin.h"
#include "VideoSourceConfigWindowBase.h"
#if defined(__LINUX__)
#include <wx/object.h>
#endif

class VideoGrabberBase;

/// Base class for all iFloor effect plugins
class IFLOOR_API VideoSourcePluginBase : public SerializableBase//wxObject
{
	DECLARE_ABSTRACT_CLASS(VideoSourcePluginBase)
public:
	virtual ~VideoSourcePluginBase();
	/// Returns GUID (unique identifier) of plugin
	/// \return string which contains unique identifier of plugin
	virtual wxString GetID() const = 0;
	/// Returns name of plugin
	/// \return string which contains human-readable name of plugin
	virtual wxString GetName() const = 0;

	virtual VideoSourceConfigWindowBase* CreateSettingsEditor(wxWindow* parent);
	virtual size_t GetDeviceCount()  const = 0;
	virtual wxString GetName(size_t index) const = 0;
	virtual wxString GetName(const wxString & id) const = 0;
	virtual wxString GetID(size_t index) const = 0;
	virtual VideoGrabberBase * CreateGrabber(size_t index) = 0;
	virtual VideoGrabberBase * CreateGrabber(const wxString & id) = 0;
	virtual void DeleteGrabber(VideoGrabberBase * grabber);
	virtual void RefreshDevices() {};
};

typedef VideoSourcePluginBase * (*CreateVideoSourcePlugin_function)();
typedef void (*DeleteVideoSourcePlugin_function)(VideoSourcePluginBase * plugin);

#endif // _VIDEOSOURCEPLUGINBASE_H