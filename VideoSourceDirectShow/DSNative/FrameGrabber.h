#ifndef __FrameGrabber_H__
#define __FrameGrabber_H__

/////////////////////////////////////////////////////
#include "Device.h"
#include "DirectShow/Include/DSFilterGraphBase.h"
#include "DirectShow/Include/DSFiltersDefinition.h"
/////////////////////////////////////////////////////
#pragma warning( disable : 4275 )
/////////////////////////////////////////////////////
class CCrossBar;
/////////////////////////////////////////////////////
class EXPORTS_API CFrameGrabber//:  public CFactoryObject
	: public DSFilterGraphBase
{
protected:
	ICaptureGraphBuilder2	* m_pCaptureGraph;
	DSSampleGrabberFilter	* m_pVideoPicker;
	DSFilterBase			* m_pNullRenderer;
	DSFilterBase			* m_pDecompressor;
	CDevice					* m_pVideoDevice;
public:
	CDevice	* get_VideoDevice();
	CCrossBar * get_Crossbar();
protected:
	DecompressorType::DECOMPTYPE ConfigureVideoSource();
protected:
	virtual HRESULT InitInterfaces();
	virtual HRESULT CloseInterfaces();
public:
	void set_PreviewWindow(HWND hWnd);
	void set_ShouldPreview(bool bShouldPreview);
	void ResizeVideoWindow();
	void ResizeVideoWindow(RECT rect);
public:
	BOOL GrabFrame(BYTE ** ppbtBuffer,long * plBufferSize);
	HRESULT get_Format(BITMAPINFOHEADER ** pbmi);
public:
	CFrameGrabber(CDevice * pVideoDevice);
	virtual ~CFrameGrabber();
};
/////////////////////////////////////////////////////
#endif // __FrameGrabber_H__