
#include "../Include/DSFilter.h"

//#define USE_NON_CRITICAL_ASSERTS

DSFilterBase::DSFilterBase()
	: DSObjectBase()
	, m_pFilter(NULL)
{
}

DSFilterBase::DSFilterBase(LPOLESTR szMoniker)
	: DSObjectBase()
	, m_pFilter(NULL)
{
	HRESULT hr = GetFilterByMoniker(szMoniker);
#ifdef USE_NON_CRITICAL_ASSERTS
	ASSERT(hr == S_OK);
#endif
}

DSFilterBase::DSFilterBase(REFCLSID iid)
	: DSObjectBase()
	, m_pFilter(NULL)
{
	HRESULT hr = CoCreateInstance(iid, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **)&m_pFilter);
#ifdef USE_NON_CRITICAL_ASSERTS
	ASSERT(hr == S_OK);
#endif
}

DSFilterBase::DSFilterBase(IBaseFilter * pFilter, bool bReleaseOnDestroy)
	: DSObjectBase()
	, m_pFilter(pFilter)
{
	m_bReleaseOnDestroy = bReleaseOnDestroy;
#ifdef USE_NON_CRITICAL_ASSERTS
	ASSERT(m_pFilter != NULL);
#endif
}

DSFilterBase::~DSFilterBase()
{
	if (m_bReleaseOnDestroy) Release();
}

HRESULT DSFilterBase::QueryInterface(const IID &riid,void ** ppvObject)
{
	return m_pFilter->QueryInterface(riid,ppvObject);
}

void DSFilterBase::Release()
{
	SAFERELEASE(m_pFilter);
}

bool DSFilterBase::HaveProperties()
{
	CComPtr< ISpecifyPropertyPages > pPropertyPages;
	HRESULT hr = S_OK;
	hr = m_pFilter->QueryInterface(IID_ISpecifyPropertyPages,(void **)&pPropertyPages);
	return SUCCEEDED(hr);
}

HRESULT DSFilterBase::ShowProperties(HWND hwndParent)
{
	CComPtr< ISpecifyPropertyPages > pPropertyPages;
	HRESULT hr = S_OK;
	hr = m_pFilter->QueryInterface(IID_ISpecifyPropertyPages,(void **)&pPropertyPages);
	if (FAILED(hr)) return hr;
	CAUUID caGUID;
	hr = pPropertyPages->GetPages(&caGUID);
	if (FAILED(hr)) return hr;

	IUnknown * pFilterUnk;
    hr = m_pFilter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);
	if (FAILED(hr)) return hr;

	FILTER_INFO fiInfo;
	hr = m_pFilter->QueryFilterInfo(&fiInfo);
	if (FAILED(hr)) return hr;

	hr = OleCreatePropertyFrame(hwndParent,0,0,fiInfo.achName,1, &pFilterUnk, caGUID.cElems,caGUID.pElems,NULL, 0, NULL);
	SAFERELEASE(pFilterUnk);
	CoTaskMemFree(caGUID.pElems);
	SAFERELEASE(fiInfo.pGraph);
	return hr;
}
bool DSFilterBase::GetIBaseFilter(IBaseFilter ** pIBaseFilter)
{
	*pIBaseFilter = m_pFilter;
	return true;
}

HRESULT DSFilterBase::AddToFilterGraph(IGraphBuilder * pGraphBuilder)
{
	ASSERT(pGraphBuilder != NULL);
	return pGraphBuilder->AddFilter(m_pFilter,NULL);
}

HRESULT DSFilterBase::RemoveFromFilterGraph(IGraphBuilder * pGraphBuilder)
{
	ASSERT(pGraphBuilder != NULL);
	return pGraphBuilder->RemoveFilter(m_pFilter);
}

IPin * DSFilterBase::GetPinByDirection(PIN_DIRECTION direction,long lPinNumber)
{
	IPin * pPin = NULL;
	HRESULT hr = S_OK;

    CComPtr<IEnumPins>  pEnumPins;

    hr = m_pFilter->EnumPins(&pEnumPins);
#ifdef USE_NON_CRITICAL_ASSERTS
	ASSERT( S_OK == hr );
#endif
    if(!FAILED(hr))
    {
        while (pEnumPins->Next(1, &pPin, NULL) == S_OK)
        {
            PIN_DIRECTION    pinDirection;
            hr = pPin->QueryDirection(&pinDirection);
            ASSERT( S_OK == hr );
            if(pinDirection != direction)
			{
				pPin->Release();
                continue;
			}
            if(lPinNumber > 0)
            {
                lPinNumber--;
				pPin->Release();
                continue;
            }
            break;
        }
		pEnumPins.Release();
    }
	return pPin;
}


IPin * DSFilterBase::GetPinByCategory(REFGUID guidPinCategory)
{
	IPin * pPin = NULL;
	HRESULT hr = S_OK;

    CComPtr<IEnumPins>  pEnumPins;

    hr = m_pFilter->EnumPins(&pEnumPins);
#ifdef USE_NON_CRITICAL_ASSERTS
	ASSERT( S_OK == hr );
#endif
    if(!FAILED(hr))
    {
        while (pEnumPins->Next(1, &pPin, NULL) == S_OK)
        {
			CComPtr<IKsPropertySet>  pPropertySet;
			hr = pPin->QueryInterface(IID_IKsPropertySet,(void**)&pPropertySet);
			if (hr == S_OK)
			{
				GUID guidOutput;
				DWORD dwReturned;
				hr = pPropertySet->Get(AMPROPSETID_Pin,AMPROPERTY_PIN_CATEGORY,NULL,NULL,(PVOID)&guidOutput,sizeof(GUID),&dwReturned);
				if (hr == S_OK && guidOutput == guidPinCategory)
				{
					pPropertySet.Release();
					break;
				}
				pPropertySet.Release();
			}
			pPin->Release();
			pPin = NULL;
        }
		pEnumPins.Release();
    }
	return pPin;
}

HRESULT DSFilterBase::GetFilterByMoniker(LPOLESTR szMoniker)
{
	HRESULT hr = S_OK;

    m_pFilter = NULL;

    CComPtr<IBindCtx> pBindCtx;
    hr = CreateBindCtx(0, &pBindCtx);
    ASSERT( S_OK == hr );

    ULONG chEaten = 0;
    CComPtr<IMoniker> pIMoniker;
    hr = MkParseDisplayName(pBindCtx, szMoniker, &chEaten, &pIMoniker);
    pBindCtx.Release();
    if (SUCCEEDED(hr))
    {
        // Get the display name, or bind to a DirectShow filter.
        hr = pIMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pFilter);
#ifdef USE_NON_CRITICAL_ASSERTS
        ASSERT( S_OK == hr );
#endif
		if (FAILED(hr)) m_pFilter = NULL;
        pIMoniker.Release();
    }
    if(m_pFilter == NULL && SUCCEEDED(hr)) hr = -1;
    return hr;
}


HRESULT DSFilterBase::GetOutputPinCapabilitiesCount(IPin * pPin, int *piCount)
{
	*piCount = -1;
	HRESULT hr;
	/*
	CComPtr<IEnumMediaTypes> pEnum;
	hr = pPin->EnumMediaTypes(&pEnum);
	if (hr == S_OK && pEnum != NULL)
	{
		pEnum->Reset();
		AM_MEDIA_TYPE * pmt = NULL;
		int iCount = 0;
		while (hr == S_OK)
		{
			hr = pEnum->Next(1,&pmt, NULL);
			if (hr == S_OK && pmt != NULL)
			{
				iCount++;
				DeleteMediaType(pmt);
			}
		}
		hr = S_OK;
		if (iCount != 0) *piCount = iCount;
		pEnum.Release();
	}
	*/
	
	CComPtr<IAMStreamConfig> pStreamConfig;
	hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pStreamConfig);
	if (hr == S_OK && pStreamConfig != NULL)
	{
		int iSize;
		hr = pStreamConfig->GetNumberOfCapabilities(piCount,&iSize);
		pStreamConfig.Release();
	}

	return hr;
}

HRESULT DSFilterBase::GetOutputPinCapabilitiesCount(long lPinNumber, int *piCount)
{
	*piCount = -1;
	IPin * pPin = GetPinByDirection(PINDIR_OUTPUT, lPinNumber);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinCapabilitiesCount(pPin,piCount);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::GetOutputPinCapabilitiesCount(REFGUID guidPinCategory, int *piCount)
{
	*piCount = -1;
	IPin * pPin = GetPinByCategory(guidPinCategory);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinCapabilitiesCount(pPin,piCount);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::GetOutputPinCapabilities(long lPinNumber, int iCapNumber, AM_MEDIA_TYPE ** ppmt)
{
	IPin * pPin = GetPinByDirection(PINDIR_OUTPUT, lPinNumber);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinCapabilities(pPin,iCapNumber,ppmt);
	pPin->Release();
	return hr;
}


HRESULT DSFilterBase::GetOutputPinCapabilities(IPin * pPin, int iCapNumber, AM_MEDIA_TYPE ** ppmt)
{
	*ppmt = NULL;	
	HRESULT hr;
/*
	CComPtr<IEnumMediaTypes> pEnum;
	hr = pPin->EnumMediaTypes(&pEnum);
	if (hr == S_OK && pEnum != NULL)
	{
		pEnum->Reset();
		AM_MEDIA_TYPE * pmt = NULL;
		while (hr == S_OK)
		{
			hr = pEnum->Next(1,&pmt, NULL);
			if (iCapNumber > 0)
			{
				iCapNumber--;
				DeleteMediaType(pmt);
			}
			else
			{
				*ppmt = pmt;
			}
		}
		if (*ppmt == NULL) hr = E_FAIL; else hr = S_OK;
		pEnum.Release();
	}
*/
	CComPtr<IAMStreamConfig> pStreamConfig;
	hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pStreamConfig);
	if (hr == S_OK && pStreamConfig != NULL)
	{
		int iCount, iSize;
		hr = pStreamConfig->GetNumberOfCapabilities(&iCount,&iSize);
		if (iCapNumber >= 0 && iCount > iCapNumber)
		{
			LPBYTE pbtSCC = (LPBYTE)CoTaskMemAlloc(iSize);
			hr = pStreamConfig->GetStreamCaps(iCapNumber,ppmt,pbtSCC);
			CoTaskMemFree(pbtSCC);
		}
		pStreamConfig.Release();
	}
	return hr;
}

HRESULT DSFilterBase::GetOutputPinCapabilities(REFGUID guidPinCategory, int iCapNumber, AM_MEDIA_TYPE ** ppmt)
{
	IPin * pPin = GetPinByCategory(guidPinCategory);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinCapabilities(pPin,iCapNumber,ppmt);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::GetOutputPinFormat(IPin * pPin, AM_MEDIA_TYPE ** ppmt)
{
	*ppmt = NULL;
	CComPtr<IAMStreamConfig> pStreamConfig;
	HRESULT hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pStreamConfig);
	if (hr == S_OK && pStreamConfig != NULL)
	{
		hr = pStreamConfig->GetFormat(ppmt);
		pStreamConfig.Release();
	}
	return hr;
}

HRESULT DSFilterBase::GetOutputPinFormat(long lPinNumber, AM_MEDIA_TYPE ** ppmt)
{
	IPin * pPin = GetPinByDirection(PINDIR_OUTPUT, lPinNumber);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinFormat(pPin,ppmt);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::GetOutputPinFormat(REFGUID guidPinCategory, AM_MEDIA_TYPE ** ppmt)
{
	IPin * pPin = GetPinByCategory(guidPinCategory);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = GetOutputPinFormat(pPin,ppmt);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::SetOutputPinFormat(long lPinNumber, AM_MEDIA_TYPE * pmt)
{
	IPin * pPin = GetPinByDirection(PINDIR_OUTPUT, lPinNumber);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = SetOutputPinFormat(pPin,pmt);
	pPin->Release();
	return hr;
}

HRESULT DSFilterBase::SetOutputPinFormat(IPin * pPin, AM_MEDIA_TYPE * pmt)
{
	CComPtr<IAMStreamConfig> pStreamConfig;
	HRESULT hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pStreamConfig);
	if (hr == S_OK && pStreamConfig != NULL)
	{
		hr = pStreamConfig->SetFormat(pmt);
		pStreamConfig.Release();
	}
	return hr;
}

HRESULT DSFilterBase::SetOutputPinFormat(REFGUID guidPinCategory, AM_MEDIA_TYPE * pmt)
{
	IPin * pPin = GetPinByCategory(guidPinCategory);
	if (pPin == NULL) return E_FAIL;
	HRESULT hr = SetOutputPinFormat(pPin,pmt);
	pPin->Release();
	return hr;
}
