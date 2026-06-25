
#include "../Include/DSFiltersDefinition.h"
#include <omp.h>

DSSampleGrabberFilter::DSSampleGrabberFilter()
	: DSFilterBase()
	, m_pISampleGrabber(NULL)
	, m_pCB(NULL)
{
	m_bReleaseOnDestroy = true;
	HRESULT hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **)&m_pFilter);
	THROWHR(hr);
	hr = m_pFilter->QueryInterface(IID_ISampleGrabber,(void**)&m_pISampleGrabber);
	THROWHR(hr);
	m_pCB = new SampleGrabberCB(m_pISampleGrabber);
	hr = m_pISampleGrabber->SetCallback(m_pCB,1);
	THROWHR(hr);
}

DSSampleGrabberFilter::~DSSampleGrabberFilter()
{
	if (m_pISampleGrabber)
	{
		m_pISampleGrabber->SetCallback(NULL,0);
		SAFERELEASE(m_pISampleGrabber);
	}
	SAFEDELETE(m_pCB);
}

HRESULT DSSampleGrabberFilter::SetMediaType(AM_MEDIA_TYPE mt)
{
	return m_pISampleGrabber->SetMediaType(&mt);
}

HRESULT DSSampleGrabberFilter::SetCallback(ISampleGrabberCB *pCallback,long WhichMethodToCallback)
{
	SAFEDELETE(m_pCB);
	return m_pISampleGrabber->SetCallback(pCallback,WhichMethodToCallback);
}

void DSSampleGrabberFilter::SetCallbackWnd( HWND wnd, UINT message )
{
	m_pCB->m_hwnd = wnd;
	m_pCB->m_Message = message;
}

HRESULT DSSampleGrabberFilter::SetCallback(CSampleCallBack	 * pCallBack)
{
	if (m_pCB)
	{
		return m_pCB->SetCallback(pCallBack);
	}
	return E_FAIL;
}
HRESULT DSSampleGrabberFilter::GetMediaType(AM_MEDIA_TYPE * pmt)
{
	CheckPointer(pmt,E_POINTER);
	if (m_pISampleGrabber)
	{
		return m_pISampleGrabber->GetConnectedMediaType(pmt);
	}
	return E_FAIL;
}

HRESULT DSSampleGrabberFilter::SetOneShot( BOOL bOneShot /*= FALSE*/ )
{
	m_pISampleGrabber->SetOneShot(bOneShot);
	return S_FALSE;
}

BOOL DSSampleGrabberFilter::Grab(BYTE ** ppbtBuffer,long * plBufferSize)
{
	if (m_pCB)
	{
		return m_pCB->Grab(ppbtBuffer,plBufferSize);
	}
	return FALSE;
}

HRESULT DSSampleGrabberFilter::SampleGrabberCB::SetCallback(CSampleCallBack	 * pCallBack)
{
	if (m_pCallBack != pCallBack)
	{
		if (WaitForSingleObject(m_hMutex,INFINITE) == WAIT_OBJECT_0)
		{
			m_bFormatSetted = FALSE;
			m_pCallBack = pCallBack;
			ReleaseMutex(m_hMutex);
			return NOERROR;
		}
	}
	return E_FAIL;
}

BOOL DSSampleGrabberFilter::SampleGrabberCB::Grab(BYTE ** ppbtBuffer,long * plBufferSize)
{
	if (WaitForSingleObject(m_hMutex,INFINITE) == WAIT_OBJECT_0)
	{
		m_bGrabbing = TRUE;
		ReleaseMutex(m_hMutex);
	}
	while(m_bGrabbing)
	{
		Sleep(50);
	}

	if (*ppbtBuffer == NULL)
	{
		*ppbtBuffer = static_cast<LPBYTE>(CoTaskMemAlloc(m_lBufferSize));
	}

	LPBYTE lpbtTemp = *ppbtBuffer;	
	DWORD _pitch = m_dwWidth * 3;

	auto segmentCount = max(1, omp_get_num_procs() / 4);
	auto segmentLength = m_dwHeight / segmentCount;
	auto i = 0;

	// Rotate and reset BGR to RGB
	#pragma omp parallel for private(i) shared(lpbtTemp)
	for (i = 0; i < segmentCount; ++i)
	{
		DWORD start = segmentLength * i;
		DWORD end = start + segmentLength;
		for (DWORD y = start; y < end; ++y)
		{
			for (DWORD x = 0; x < _pitch; x += 3)
			{
				lpbtTemp[y * _pitch + x] = m_pbtBuffer[(m_dwHeight - (y + 1)) * _pitch + x + 2];
				lpbtTemp[y * _pitch + x + 1] = m_pbtBuffer[(m_dwHeight - (y + 1)) * _pitch + x + 1];
				lpbtTemp[y * _pitch + x + 2] = m_pbtBuffer[(m_dwHeight - (y + 1)) * _pitch + x];
			}
		}
	}
	// Rotate
	/*
	for (DWORD y = 0; y < m_dwHeight; y++)
	{
		for (DWORD x = 0;x < _pitch; x++)
		{
			lpbtTemp[y * _pitch + x] = m_pbtBuffer[(m_dwHeight - (y + 1)) * _pitch + x];
		}
	}
	*/

	*plBufferSize = m_lBufferSize;
	return TRUE;
}

HRESULT STDMETHODCALLTYPE DSSampleGrabberFilter::SampleGrabberCB::BufferCB(double SampleTime,BYTE *pBuffer,long BufferLen)
{
	if (WaitForSingleObject(m_hMutex,INFINITE) == WAIT_OBJECT_0)
	{
		if (!m_bFormatSetted)
		{
			if (m_pISampleGrabber)
			{
				AM_MEDIA_TYPE mt;
				if (SUCCEEDED(m_pISampleGrabber->GetConnectedMediaType(&mt)))
				{
					VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*) mt.pbFormat;
					m_dwWidth = vih->bmiHeader.biWidth;
					m_dwHeight = vih->bmiHeader.biHeight;
					if (m_pCallBack)
					{
						m_pCallBack->OnFormat(mt.formattype,mt.pbFormat,mt.cbFormat);
					}
					m_bFormatSetted = TRUE;
					FreeMediaType(mt);
				}
			}
		}
		if (m_pCallBack)
		{
			m_pCallBack->OnSample(pBuffer,BufferLen);
			m_pCallBack->m_dTimeStamp = SampleTime;	
		}
		if (m_bGrabbing)
		{
			if (m_lBufferSize != BufferLen)
			{
				SAFECOTASKFREE(m_pbtBuffer);
			}
			if (!m_pbtBuffer)
			{
				m_pbtBuffer = (LPBYTE)CoTaskMemAlloc(BufferLen);
			}
			m_lBufferSize = BufferLen;
			CopyMemory(m_pbtBuffer,pBuffer,m_lBufferSize);
			m_bGrabbing = FALSE;
		}
		ReleaseMutex(m_hMutex);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DSSampleGrabberFilter::SampleGrabberCB::SampleCB(double SampleTime,IMediaSample *pSample)
{
	return S_OK;
}