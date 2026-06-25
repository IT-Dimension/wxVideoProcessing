#ifndef _VIDEOGRABBERBASE_H
#define _VIDEOGRABBERBASE_H

#include "VideoSourcePlugin.h"
#include <SerializableBase.h>

class VideoSourcePluginBase;

class IFLOOR_API VideoGrabberBase : public SerializableBase
{
	DECLARE_ABSTRACT_CLASS(VideoGrabberBase)
public:
	VideoGrabberBase();
	VideoGrabberBase(VideoSourcePluginBase * owner);
	virtual ~VideoGrabberBase();
	virtual wxSize GetFrameSize() = 0;
	virtual size_t GetFrameDataLength() = 0;
	virtual unsigned char * GetBuffer() = 0;
	virtual unsigned char * GrabFrame(bool bUseInternalBuffer = true) = 0;
	virtual int GrabFrame(unsigned char ** data, long * length) = 0;
	virtual bool IsOK() = 0;
	VideoSourcePluginBase * GetVideoSource();
	virtual CommonConfigWindowBase * CreateSettingsEditor(wxWindow * parent = NULL){return NULL;};

protected:
	VideoSourcePluginBase * m_VideoSource;
};

WX_DECLARE_USER_EXPORTED_LIST(VideoGrabberBase, VideoGrabberBaseList, IFLOOR_API); //-V521

#endif // _VIDEOGRABBERBASE_H