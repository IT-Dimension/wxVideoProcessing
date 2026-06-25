#include "stdwx.h"
#include "FrameGrabber.h"
#include "Crossbar.h"
#include <dvdmedia.h>

CFrameGrabber::CFrameGrabber(CDevice * pVideoDevice)
	//: CFactoryObject()
	: m_pVideoDevice(pVideoDevice)
	, m_pCaptureGraph(NULL)
	, m_pVideoPicker(NULL)
	, m_pNullRenderer(NULL)
	, m_pDecompressor(NULL)
{

}

CFrameGrabber::~CFrameGrabber()
{
	CloseInterfaces();
}

CDevice	* CFrameGrabber::get_VideoDevice()
{
	return m_pVideoDevice;
}

HRESULT CFrameGrabber::InitInterfaces()
{
	HRESULT hr = __super::InitInterfaces();
	SAFECHECKCLOSE(hr);

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, 
		IID_ICaptureGraphBuilder2, (void **)&m_pCaptureGraph);
	SAFECHECKCLOSE(hr);

	hr = m_pCaptureGraph->SetFiltergraph(m_pGraphBuilder);
	SAFECHECKCLOSE(hr);

	if (m_pVideoDevice && m_pVideoDevice->m_pFilter)
	{
		hr = m_pVideoDevice->m_pFilter->AddToFilterGraph(m_pGraphBuilder);
		SAFECHECKCLOSE(hr);		

		DecompressorType::DECOMPTYPE _decompressorType = ConfigureVideoSource();

		IBaseFilter * pDecompressor = NULL;
		if (_decompressorType == DecompressorType::AVI)
		{
			m_pDecompressor = new DSAviDecompressorFilter();
			m_pDecompressor->GetIBaseFilter(&pDecompressor);
		} else
			if (_decompressorType == DecompressorType::MJPEG)
		{
			m_pDecompressor = new DSMJPEGDecompressorFilter();
			m_pDecompressor->GetIBaseFilter(&pDecompressor);
		}

		m_pVideoPicker = new DSSampleGrabberFilter();
		AM_MEDIA_TYPE mt;
		ZeroMemory(&mt,sizeof(mt));
		mt.majortype = MEDIATYPE_Video;
		mt.subtype = MEDIASUBTYPE_RGB24;
		m_pVideoPicker->SetMediaType(mt);
		m_pVideoPicker->AddToFilterGraph(m_pGraphBuilder);
		IBaseFilter * pPicker;
		m_pVideoPicker->GetIBaseFilter(&pPicker);

		if (pDecompressor)
		{
			m_pDecompressor->AddToFilterGraph(m_pGraphBuilder);

			IBaseFilter * pSource;
			m_pVideoDevice->m_pFilter->GetIBaseFilter(&pSource);
			hr = m_pCaptureGraph->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,pSource,pDecompressor,pPicker);
			//hr = m_pCaptureGraph->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,pSource,pDecompressor,pPicker);
			if (FAILED(hr))
			{
				CloseInterfaces();
				return hr;
			}
			hr = m_pCaptureGraph->RenderStream(NULL , NULL ,pPicker,NULL,NULL);
			if (FAILED(hr))
			{
				CloseInterfaces();
				return hr;
			}
		}
		else
		{
			IBaseFilter * pSource;
			m_pVideoDevice->m_pFilter->GetIBaseFilter(&pSource);
			hr = m_pCaptureGraph->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,pSource,pPicker,NULL);
			
			//m_pNullRenderer = new DSNullRenderer();
			/*
			m_pNullRenderer->AddToFilterGraph(m_pGraphBuilder);

			IBaseFilter * pNullRenderer;
			m_pNullRenderer->GetIBaseFilter(&pNullRenderer);
			hr = m_pCaptureGraph->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,pSource,pPicker,pNullRenderer);
			if (FAILED(hr))
			{
				CloseInterfaces();
				return hr;
			}
			*/
		}
	}
	return PreparePlayback();
}

HRESULT CFrameGrabber::CloseInterfaces()
{
	if (m_pVideoDevice && m_pVideoDevice->m_pFilter && m_pGraphBuilder)
	{
		m_pVideoDevice->m_pFilter->RemoveFromFilterGraph(m_pGraphBuilder);
	}
	SAFEDELETE(m_pNullRenderer);
	SAFERELEASE(m_pCaptureGraph);
	SAFEDELETE(m_pDecompressor);
	SAFEDELETE(m_pVideoPicker);
	return __super::CloseInterfaces();
}

DecompressorType::DECOMPTYPE CFrameGrabber::ConfigureVideoSource()
{
	DecompressorType::DECOMPTYPE _decompressorType = DecompressorType::None;
	if (m_pVideoDevice && m_pVideoDevice->m_pFilter)
	{
		IPin * pPin = NULL;
		pPin = m_pVideoDevice->m_pFilter->GetPinByCategory(PIN_CATEGORY_CAPTURE);
		if (!pPin) pPin = m_pVideoDevice->m_pFilter->GetPinByDirection(PINDIR_OUTPUT,0);
		if (pPin)
		{
			AM_MEDIA_TYPE * pmt;
			HRESULT hr = m_pVideoDevice->m_pFilter->GetOutputPinFormat(pPin,&pmt);
			if (hr == S_OK)
			{
				if ( pmt->formattype == FORMAT_VideoInfo)
				{
					GUID _fmtSubType = pmt->subtype;
					hr = E_FAIL;
					if (hr != S_OK)
					{
						if (_fmtSubType != MEDIASUBTYPE_YUY2) pmt->subtype = MEDIASUBTYPE_YUY2;
						hr = m_pVideoDevice->m_pFilter->SetOutputPinFormat(pPin,pmt);
						_decompressorType = DecompressorType::AVI;
					}
					if (hr != S_OK)
					{
						if (_fmtSubType != MEDIASUBTYPE_RGB32) pmt->subtype = MEDIASUBTYPE_RGB32;
						hr = m_pVideoDevice->m_pFilter->SetOutputPinFormat(pPin,pmt);
						_decompressorType = DecompressorType::None;
					}
					if (hr != S_OK)
					{
						if (_fmtSubType != MEDIASUBTYPE_RGB24) pmt->subtype = MEDIASUBTYPE_RGB24;
						hr = m_pVideoDevice->m_pFilter->SetOutputPinFormat(pPin,pmt);
						_decompressorType = DecompressorType::None;
					}
					if (hr != S_OK)
					{
						if (_fmtSubType != MEDIASUBTYPE_MJPG) pmt->subtype = MEDIASUBTYPE_MJPG;
						hr = m_pVideoDevice->m_pFilter->SetOutputPinFormat(pPin,pmt);
						_decompressorType = DecompressorType::MJPEG;
					}
					if (hr != S_OK)
					{
						_decompressorType = DecompressorType::None;
					}
				}
				DeleteMediaType(pmt);
			}
			pPin->Release();
		}
	}	
	return _decompressorType;
}
void CFrameGrabber::set_PreviewWindow(HWND hWnd)
{
	__super::set_PreviewWindow(hWnd);
}

void CFrameGrabber::set_ShouldPreview(bool bShouldPreview)
{
	__super::set_ShouldPreview(bShouldPreview);
}

void CFrameGrabber::ResizeVideoWindow()
{
	__super::ResizeVideoWindow();
}

void CFrameGrabber::ResizeVideoWindow(RECT rect)
{
	__super::ResizeVideoWindow(rect);
}

CCrossBar * CFrameGrabber::get_Crossbar()
{
	if (m_pCaptureGraph && m_pVideoPicker)
	{
		IBaseFilter * pPicker;
		m_pVideoPicker->GetIBaseFilter(&pPicker);
		IAMCrossbar *pICrossBar = NULL;
		if (SUCCEEDED(m_pCaptureGraph->FindInterface(&LOOK_UPSTREAM_ONLY,NULL,pPicker,IID_IAMCrossbar,(void**)&pICrossBar)))
		{
			CCrossBar * pCrossbar = new CCrossBar(pICrossBar);
			pICrossBar->Release();
			return pCrossbar;
		}
	}
	return NULL;
}

HRESULT CFrameGrabber::get_Format(BITMAPINFOHEADER ** pbmi)
{
	CheckPointer(pbmi,E_POINTER);
	*pbmi = NULL;
	if (m_pVideoPicker)
	{
		AM_MEDIA_TYPE mt;
		HRESULT hr = m_pVideoPicker->GetMediaType(&mt);
		if (FAILED(hr)) return hr;
		if (mt.formattype == FORMAT_VideoInfo)
		{
			VIDEOINFO * pvi = (VIDEOINFO *)mt.pbFormat;
			*pbmi = (BITMAPINFOHEADER *)CoTaskMemAlloc(sizeof(pvi->bmiHeader));
			CopyMemory(*pbmi,&pvi->bmiHeader,sizeof(pvi->bmiHeader));
		}
		if (mt.formattype == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2 * pvi = (VIDEOINFOHEADER2 *)mt.pbFormat;
			*pbmi = (BITMAPINFOHEADER *)CoTaskMemAlloc(sizeof(pvi->bmiHeader));
			CopyMemory(*pbmi,&pvi->bmiHeader,sizeof(pvi->bmiHeader));
		}
		FreeMediaType(mt);
		if (*pbmi == NULL) return E_UNEXPECTED;
		return NOERROR;
	}
	return E_FAIL;
}

BOOL CFrameGrabber::GrabFrame(BYTE ** ppbtBuffer,long * plBufferSize)
{
	if (m_pVideoPicker)
	{
		return m_pVideoPicker->Grab(ppbtBuffer,plBufferSize);
	}
	return FALSE;
}