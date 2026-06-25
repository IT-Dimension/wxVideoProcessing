
#include "../Include/DSCategoryHelper.h"

DSCategory::DSCategory(REFCLSID clsidCategory)
	: DSObjectBase()
	, m_clsidCategory(clsidCategory)
	, m_pDevEnum(NULL)
	, m_pEnumMoniker(NULL)
{
	HRESULT hr = CoCreateInstance(
                        CLSID_SystemDeviceEnum, 
                        NULL, 
                        CLSCTX_INPROC_SERVER,
                        IID_ICreateDevEnum, 
                        (void**) &m_pDevEnum);

	ASSERT(hr == S_OK && m_pDevEnum != NULL);

    hr = m_pDevEnum->CreateClassEnumerator(
                        m_clsidCategory,
                        &m_pEnumMoniker, 
                        0);

    //ASSERT(hr == S_OK && m_pEnumMoniker != NULL);
}

DSCategory::~DSCategory()
{
}

void DSCategory::Release()
{
	SAFERELEASE(m_pDevEnum);
	SAFERELEASE(m_pEnumMoniker);
}

HRESULT DSCategory::QueryInterface(const IID &riid,void ** ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	if (m_pEnumMoniker)
	{
		hr = m_pEnumMoniker->QueryInterface(riid,ppvObject);
	}
	if (hr == S_OK) return hr;
	if (m_pDevEnum) return m_pDevEnum->QueryInterface(riid,ppvObject);
	return E_NOINTERFACE;
}

HRESULT DSCategory::GetFiltersCount(long & lCount)
{
	lCount = 0;
	if (!m_pEnumMoniker) return NOERROR;
	m_pEnumMoniker->Reset();
    while(1)
    {
        ULONG cFetched = 0;
        CComPtr< IMoniker > pMoniker;

        HRESULT hr = m_pEnumMoniker->Next(1, &pMoniker, &cFetched);
        if(!pMoniker || hr != S_OK) break;
        
        CComPtr< IPropertyBag > pBag;
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**) &pBag);
        if(!FAILED(hr))
        {
			lCount++;
			pBag.Release();
        }
		pMoniker.Release();
    }
	
    return S_OK;
}

HRESULT DSCategory::GetFilterName(long lNumber, LPWSTR * plpwstrDevName)
{
	* plpwstrDevName = NULL;
	HRESULT hr = S_OK;
	if (!m_pEnumMoniker) return E_FAIL;
	m_pEnumMoniker->Reset();
    while(1)
    {
        ULONG cFetched = 0;
        CComPtr< IMoniker > pMoniker;

        hr = m_pEnumMoniker->Next(1, &pMoniker, &cFetched);
        if(!pMoniker || hr != S_OK) break;
		if (lNumber > 0)
		{
			lNumber--;
			pMoniker.Release();
			continue;
		}
		
		CComPtr< IPropertyBag > pBag;
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**) &pBag);
        if(!FAILED(hr))
        {
            VARIANT var;
			VariantInit(&var);
            //var.vt = VT_BSTR;
			//hr = pBag->Read(L"FilterData",&var, NULL);
			//if (S_OK == hr) VariantClear(&var);
			//hr = pBag->Read(L"CLSID",&var, NULL);
			//if (S_OK == hr) VariantClear(&var);
            hr = pBag->Read(L"FriendlyName",&var, NULL);
            if(hr == NOERROR)
            {
				*plpwstrDevName = new WCHAR[MAX_PATH]; 
				wcscpy_s(*plpwstrDevName, MAX_PATH, var.bstrVal);
				//SysFreeString(var.bstrVal);
				VariantClear(&var);
			}
			pBag.Release();
		}
		pMoniker.Release();
		break;
    }
	if (!(*plpwstrDevName)) hr = E_FAIL;
    return hr;
}

HRESULT DSCategory::GetDeviceFilter(long lNumber, IBaseFilter ** ppOutFilter)
{
	HRESULT hr = S_OK;
	(*ppOutFilter) = NULL;
	if (!m_pEnumMoniker) return E_FAIL;
	m_pEnumMoniker->Reset();
	while(1)
    {
        ULONG cFetched = 0;
        CComPtr< IMoniker > pMoniker;

        hr = m_pEnumMoniker->Next(1, &pMoniker, &cFetched);
        if(!pMoniker || hr != S_OK) break;
		if (lNumber > 0)
		{
			lNumber--;
			pMoniker.Release();
			continue;
		}
		CComPtr< IPropertyBag > pBag;
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**) &pBag);
        if(SUCCEEDED(hr))
        {
            VARIANT var;
			VariantInit(&var);
            hr = pBag->Read(L"FriendlyName",&var, NULL);
            if(FAILED(hr))
            {
				hr = pBag->Read(L"Description",&var, NULL);
			}
			VariantClear(&var);
			pBag.Release();
			if (SUCCEEDED(hr))
			{
				hr = BindMoniker(pMoniker,0,IID_IBaseFilter, (void**)ppOutFilter);
				//hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)ppOutFilter);
			}
		}
		pMoniker.Release();
		break;
    }
	if (hr == S_OK && (*ppOutFilter) == NULL) hr = E_FAIL;
    return hr;
}

HRESULT DSCategory::GetFilterMonikerString(long lNumber, LPWSTR * plpwstrDevMoniker)
{
	* plpwstrDevMoniker = NULL;
	HRESULT hr = S_OK;
	if (!m_pEnumMoniker) return E_FAIL;
	m_pEnumMoniker->Reset();
    while(1)
    {
        ULONG cFetched = 0;
        CComPtr< IMoniker > pMoniker;

        hr = m_pEnumMoniker->Next(1, &pMoniker, &cFetched);
        if(!pMoniker || hr != S_OK) break;
		if (lNumber > 0)
		{
			lNumber--;
			pMoniker.Release();
			continue;
		}
		/*
		CLSID _clsid;
		hr = pMoniker->GetClassID(&_clsid);
		_clsid = GUID_NULL;

		*/
		CComPtr<IBindCtx> pBindCtx = NULL;
		hr = CreateBindCtx(NULL,&pBindCtx);
		
		LPOLESTR	lpolestrTemp	= NULL;
		hr = pMoniker->GetDisplayName(pBindCtx,NULL,&lpolestrTemp);

		*plpwstrDevMoniker = new WCHAR[MAX_PATH]; 
		wcscpy_s(*plpwstrDevMoniker, MAX_PATH, lpolestrTemp);
		//*plpwstrDevMoniker
		
		pBindCtx.Release();
		pMoniker.Release();
		break;
    }
	if (!(*plpwstrDevMoniker)) hr = E_FAIL;
    return hr;
}
long DSCategory::get_Count()
{
	long lResult = -1;
	if (GetFiltersCount(lResult) == S_OK) return lResult;
	return -1;
}

LPWSTR DSCategory::GetName(long lNumber)
{
	LPWSTR  lpwstrName = NULL;
	if (GetFilterName(lNumber,(WCHAR**)&lpwstrName) == S_OK) return lpwstrName;
	return NULL;
}

DSFilterBase * DSCategory::GetDevice(long lNumber)
{
	IBaseFilter * pFilter = NULL;
	if (SUCCEEDED(GetDeviceFilter(lNumber, &pFilter)))
	{
		return new DSFilterBase(pFilter);
	}
	return NULL;
}

LPWSTR DSCategory::GetDeviceMonikerString(long lNumber)
{
	LPWSTR		lpwstrName		= NULL;
	if (GetFilterMonikerString(lNumber,(WCHAR**)&lpwstrName) == S_OK) 
	{
		return lpwstrName;
	}
	return NULL;
}
