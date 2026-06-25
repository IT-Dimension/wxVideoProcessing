#include "stdwx.h"
#include "DeviceManager.h"
#include "VideoSourceDirectShowPlugin.h"
#include "VideoGrabberDirectShow.h"

IMPLEMENT_DYNAMIC_CLASS(VideoSourceDirectShowPlugin, VideoSourcePluginBase)

VideoSourceDirectShowPlugin::VideoSourceDirectShowPlugin()
: m_ID(wxT("{DEADA8F3-62BB-45d5-9F04-45AC1FF3A996}"))
, m_pDeviceManager(NULL)
{
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	m_pDeviceManager = new CDeviceManager(CLSID_VideoInputDeviceCategory);
}

VideoSourceDirectShowPlugin::~VideoSourceDirectShowPlugin()
{
	wxDELETE(m_pDeviceManager);
	CoUninitialize();
}

wxString VideoSourceDirectShowPlugin::GetID() const
{
	return m_ID;
}

wxString VideoSourceDirectShowPlugin::GetName() const
{
	return _("DirectShow Video Source");
}

size_t VideoSourceDirectShowPlugin::GetDeviceCount() const
{
	return m_pDeviceManager->get_Count();
}

wxString VideoSourceDirectShowPlugin::GetName(size_t index) const
{
	LPWSTR lpwstrName = m_pDeviceManager->get_DeviceName(index);
	wxString name(lpwstrName);
	wxDELETEA(lpwstrName);
	return name;
}

wxString VideoSourceDirectShowPlugin::GetName(const wxString & id) const
{
	long lCount = m_pDeviceManager->get_Count();
	while (lCount > 0)
	{
		lCount--;
		wxString _string = GetID(lCount);
		if (_string == id) return GetName(lCount);
	}
	return wxEmptyString;
}

wxString VideoSourceDirectShowPlugin::GetID(size_t index) const
{
	LPWSTR lpwstrName = m_pDeviceManager->get_DeviceMonikerString(index);
	wxString name(lpwstrName);
	wxDELETEA(lpwstrName);
	return name;
}

VideoGrabberBase * VideoSourceDirectShowPlugin::CreateGrabber(size_t index)
{
	VideoGrabberDirectShow * grabber = new VideoGrabberDirectShow(this, index);
	if (!grabber->IsOK())
		wxDELETE(grabber);
	return grabber;
}

VideoGrabberBase * VideoSourceDirectShowPlugin::CreateGrabber(const wxString & id)
{
	VideoGrabberDirectShow * grabber = new VideoGrabberDirectShow(this, id);
	if (!grabber->IsOK())
		wxDELETE(grabber);
	return grabber;
}

void VideoSourceDirectShowPlugin::DeleteGrabber(VideoGrabberBase * grabber)
{
	wxLogDebug(wxT("VideoSourceDirectShowPlugin::DeleteGrabber"));
	wxDELETE(grabber);
}

void VideoSourceDirectShowPlugin::RefreshDevices()
{
	m_pDeviceManager->RefreshDevices();
}