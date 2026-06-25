#include "stdwx.h"
#include "VideoGrabberBase.h"
#include "VideoSourcePluginBase.h"
#include <wx/listimpl.cpp>

IMPLEMENT_ABSTRACT_CLASS(VideoGrabberBase, wxObject)

WX_DEFINE_USER_EXPORTED_LIST(VideoGrabberBaseList);

VideoGrabberBase::VideoGrabberBase()
: m_VideoSource(NULL)
{
}

VideoGrabberBase::VideoGrabberBase(VideoSourcePluginBase * owner)
: m_VideoSource(owner)
{
}

VideoGrabberBase::~VideoGrabberBase()
{
}

VideoSourcePluginBase * VideoGrabberBase::GetVideoSource()
{
	return m_VideoSource;
}
