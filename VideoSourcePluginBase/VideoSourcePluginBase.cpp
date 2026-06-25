#include "stdwx.h"
#include "VideoSourcePluginBase.h"
#include "VideoGrabberBase.h"

IMPLEMENT_ABSTRACT_CLASS(VideoSourcePluginBase, SerializableBase)

VideoSourcePluginBase::~VideoSourcePluginBase()
{
}

void VideoSourcePluginBase::DeleteGrabber(VideoGrabberBase * grabber)
{
	wxDELETE(grabber);
}

VideoSourceConfigWindowBase* VideoSourcePluginBase::CreateSettingsEditor(wxWindow* parent)
{
	return new VideoSourceConfigWindowBase(this, parent);
};