
#include "stdwx.h"
#include "Device.h"
#include "FrameGrabber.h"
#include "Format.h"

CDevice::CDevice(LPOLESTR szMoniker)
	//: CFactoryObject()
	: m_lpwstrMoniker(NULL)
{
	m_pFilter = new DSFilterBase(szMoniker);
	IBaseFilter * pFilter = NULL;
	BOOL bFailed = TRUE;
	if (m_pFilter->GetIBaseFilter(&pFilter))
	{
		bFailed = (pFilter == NULL);
	}
	if (bFailed)
	{
		SAFEDELETE(m_pFilter);
	}
	else
	{
		set_Moniker(szMoniker);
	}
}

CDevice::CDevice(REFCLSID iid)
	//: CFactoryObject()
	: m_lpwstrMoniker(NULL)
{
	m_pFilter = new DSFilterBase(iid);
	IBaseFilter * pFilter = NULL;
	BOOL bFailed = TRUE;
	if (m_pFilter->GetIBaseFilter(&pFilter))
	{
		bFailed = (pFilter == NULL);
	}
	if (bFailed)
	{
		SAFEDELETE(m_pFilter);
	}
}

CDevice::CDevice(DSFilterBase * pFilter)
	//: CFactoryObject()
	: m_pFilter(pFilter)
	, m_lpwstrMoniker(NULL)
{
	IBaseFilter * pBaseFilter = NULL;
	BOOL bFailed = TRUE;
	if (m_pFilter->GetIBaseFilter(&pBaseFilter))
	{
		bFailed = (pBaseFilter == NULL);
	}
	if (bFailed)
	{
		SAFEDELETE(m_pFilter);
	}
}

CDevice::~CDevice()
{
	SAFEDELETEARRAY(m_lpwstrMoniker);
	SAFEDELETE(m_pFilter);
}

HRESULT CDevice::ShowProperties(HWND hwndParent)
{
	if (m_pFilter)
	{
		return m_pFilter->ShowProperties(hwndParent);
	}
	return E_FAIL;
}

BOOL CDevice::HaveProperties()
{
	if (m_pFilter)
	{
		return m_pFilter->HaveProperties()? TRUE : FALSE;
	}
	return FALSE;
}
LPWSTR CDevice::get_Moniker()
{
	return m_lpwstrMoniker;
}

void CDevice::set_Moniker(LPCWSTR lpwstrMoniker)
{
	SAFEDELETEARRAY(m_lpwstrMoniker);
	if (lpwstrMoniker)
	{
		int cch = (int)(wcslen(lpwstrMoniker) + 1);
		m_lpwstrMoniker = new WCHAR[cch];
		ZeroMemory(m_lpwstrMoniker,cch * sizeof(WCHAR));
		wcscpy_s(m_lpwstrMoniker, cch, lpwstrMoniker);
	}
}
CFrameGrabber * CDevice::CreateFrameGrabber()
{
	return new CFrameGrabber(this);
}

CVideoFormats * CDevice::get_VideoFormats()
{
	CVideoFormats * pFormats = NULL;
	if (m_pFilter)
	{
		IAMStreamConfig * pStreamConfig = NULL;
		IPin * pPin = m_pFilter->GetPinByCategory(PIN_CATEGORY_CAPTURE);
		if (pPin)
		{
			if (SUCCEEDED(pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pStreamConfig)))
			{
				pFormats = new CVideoFormats(pStreamConfig);
				pStreamConfig->Release();
			}
			pPin->Release();
		}
	}
	return pFormats;
}