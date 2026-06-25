#ifndef _VIDEOGRABBERDIRECTSHOW_H
#define _VIDEOGRABBERDIRECTSHOW_H

#include <VideoGrabberBase.h>
#include "FrameGrabber.h"
#include "DeviceManager.h"
#include "Format.h"

class VideoGrabberDirectShow : public VideoGrabberBase
{
private:
	CDeviceManager m_DeviceManager;
	CDevice * m_pDevice;
	CFrameGrabber * m_pGrabber;
	int m_DeviceIndex;
	wxString m_DeviceID;
	wxString m_DeviceName;
	wxSize m_ImageSize;
	CCritSec m_csLock;
	BITMAPINFOHEADER * m_pbmi;
	LPBYTE m_pBuffer;
private:
	bool StopGrabber();
	bool StartGrabber();
	void ReadName();
public:
	// TODO: make it compatible with your plugin
	virtual size_t get_ResolutionsCount();
	virtual wxSize get_ResolutionByIndex(size_t nIndex);
	virtual bool set_Resolution(wxSize _size);
	virtual bool set_Resolution(size_t nIndex);
public:
	virtual wxString GetName() const;
	virtual wxString GetID() const;
	virtual wxSize GetFrameSize();
	virtual size_t GetFrameDataLength();
	virtual unsigned char * GetBuffer();
	virtual unsigned char * GrabFrame(bool bUseInternalBuffer = true);
	virtual int GrabFrame(unsigned char ** data, long * length);
	virtual bool IsOK();

public:
	VideoGrabberDirectShow();
	VideoGrabberDirectShow(VideoSourcePluginBase * owner, size_t index);
	VideoGrabberDirectShow(VideoSourcePluginBase * owner, const wxString & id);
	~VideoGrabberDirectShow();
};

#endif // _VIDEOGRABBERDIRECTSHOW_H