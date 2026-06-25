
#ifndef __DS_FILTERS_DEFINITION_H__
#define __DS_FILTERS_DEFINITION_H__

#include "DSFilter.h"

#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include <Qedit.h>

#define SAMPLE_GETTED WM_USER + 500

class DecompressorType
{
public:
	typedef enum {
		None	= 0,
		AVI		= 1,
		MJPEG	= 2
	} DECOMPTYPE, * LPDECOMPTYPE;
private:
	DecompressorType() {}
	~DecompressorType() {}
};

// Inf Tee Filter
class DSInfTeeFilter : public DSFilterBase
{
public:
	DSInfTeeFilter():DSFilterBase(CLSID_InfTee)
	{
	}
};

class DSSmartTeeFilter : public DSFilterBase
{
public:
	DSSmartTeeFilter():DSFilterBase(CLSID_SmartTee)
	{
	}
};

class DSFileWritterBase: public DSFilterBase
{
public:
	HRESULT SetOutputFileName(LPWSTR lpwstrFileName);
public:
	DSFileWritterBase(LPOLESTR szMoniker);
	DSFileWritterBase(REFCLSID iid);
};

// File Writer Filter
class DSFileWriter: public DSFileWritterBase
{
public:
	DSFileWriter():DSFileWritterBase(CLSID_FileWriter)
	{
	}
};

// DSound Renderer
class DSDSoundRendererFilter : public DSFilterBase
{
public:
	DSDSoundRendererFilter():DSFilterBase(CLSID_DSoundRender)
	{
	}
};

// Sample Grabber
class CSampleCallBack
{
protected:
	GUID	m_FormatType;
	
	LPBYTE	m_lpFormat;
	
public:
	double	m_dTimeStamp;
	virtual void OnFormat(GUID guidFormatType,BYTE *pbFormat,long lFormatLength)
	{
		SAFECOTASKFREE(m_lpFormat);
		m_FormatType = guidFormatType;
		m_lpFormat = (LPBYTE)CoTaskMemAlloc(lFormatLength);
		CopyMemory(m_lpFormat,pbFormat,lFormatLength);
	}
	virtual void OnSample(BYTE *pBuffer,long lBufferLen)PURE;
public:
	CSampleCallBack()
		:m_lpFormat(NULL)
		,m_FormatType(GUID_NULL)
	{

	}
	virtual ~CSampleCallBack()
	{
		SAFECOTASKFREE(m_lpFormat);
	}
	virtual HRESULT GetSampleTime(double& time)PURE;
};

class DSSampleGrabberFilter : public DSFilterBase
{
private:
	class SampleGrabberCB:public ISampleGrabberCB
	{
	public:
		HWND					m_hwnd;
		UINT					m_Message;
	private:
		ISampleGrabber		*	m_pISampleGrabber;
		HANDLE					m_hMutex;
		BOOL					m_bGrabbing;
		BYTE				*	m_pbtBuffer;
		long					m_lBufferSize;
		DWORD					m_dwWidth;
		DWORD					m_dwHeight;
		CSampleCallBack		*	m_pCallBack;
		BOOL					m_bFormatSetted;
	public:
		STDMETHODIMP_(ULONG) AddRef() { return 2; }
		STDMETHODIMP_(ULONG) Release() { return 1; }
		STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
		{
			if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
			{
				*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
				return NOERROR;
			}
			return E_NOINTERFACE;
		}
	public:
		virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime,BYTE *pBuffer,long BufferLen);
		virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime,IMediaSample *pSample);
	public:
		HRESULT SetCallback(CSampleCallBack	 * pCallBack);
		BOOL Grab(BYTE ** ppbtBuffer,long * plBufferSize);
	public:
		SampleGrabberCB(ISampleGrabber * pISampleGrabber)
			: m_pISampleGrabber(pISampleGrabber)
			, m_hMutex(INVALID_HANDLE_VALUE)
			, m_bGrabbing(FALSE)
			, m_pbtBuffer(NULL)
			, m_lBufferSize(0)
			, m_dwWidth(0)
			, m_dwHeight(0)
			, m_pCallBack(NULL)
			, m_bFormatSetted(FALSE)
		{
			m_hMutex = CreateMutex(NULL,FALSE,NULL);
		}
		~SampleGrabberCB()
		{
			m_pCallBack = NULL;
			SAFECOTASKFREE(m_pbtBuffer);
			m_lBufferSize = 0;
			CloseHandle(m_hMutex);
			m_hMutex = INVALID_HANDLE_VALUE;
		}
	};
protected:
	SampleGrabberCB * m_pCB;
	ISampleGrabber	* m_pISampleGrabber;
public:
	BOOL Grab(BYTE ** ppbtBuffer,long * plBufferSize);
	HRESULT SetMediaType(AM_MEDIA_TYPE mt);
	HRESULT GetMediaType(AM_MEDIA_TYPE * pmt);
	HRESULT SetCallback(CSampleCallBack	 * pCallBack);
	HRESULT SetCallback(ISampleGrabberCB *pCallback,long WhichMethodToCallback = 0);
	HRESULT SetOneShot(BOOL bOneShot = FALSE);
	void	SetCallbackWnd(HWND wnd, UINT message);
	bool IsFrameReady();
public:
	DSSampleGrabberFilter();
	virtual ~DSSampleGrabberFilter();
};

// WM ASF Writer
class DSWMAsfWritter : public DSFileWritterBase
{
public:
	DSWMAsfWritter(): DSFileWritterBase(CLSID_WMAsfWriter)
	{
	}
};

// Default Video Renderer
class DSVideoRenderer: public DSFilterBase
{
public:
	DSVideoRenderer():DSFilterBase(CLSID_VideoRenderer)
	{
	}
};

// VMR Renderer
class DSVMRRenderer: public DSFilterBase
{
public:
	DSVMRRenderer():DSFilterBase(CLSID_VideoMixingRenderer9)
	{
	}
};

// Avi Muxer
class DSAviMuxFilter: public DSFilterBase
{
public:
	DSAviMuxFilter():DSFilterBase(CLSID_AviDest)
	{
	}
};

// Avi Decompressor
class DSAviDecompressorFilter: public DSFilterBase
{
public:
	DSAviDecompressorFilter():DSFilterBase(CLSID_AVIDec)
	{
	}
};

// MJPEG Decompressor Filter
class DSMJPEGDecompressorFilter: public DSFilterBase
{
public:
	DSMJPEGDecompressorFilter():DSFilterBase(CLSID_MjpegDec)
	{
	}
};

class DSNullRenderer: public DSFilterBase
{
public:
	DSNullRenderer():DSFilterBase(CLSID_NullRenderer)
	{
	}
};

#endif