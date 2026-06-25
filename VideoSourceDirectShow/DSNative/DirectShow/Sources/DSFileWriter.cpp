
#include "../Include/DSFiltersDefinition.h"

DSFileWritterBase::DSFileWritterBase(LPOLESTR szMoniker)
	: DSFilterBase(szMoniker)
{
	m_bReleaseOnDestroy = true;
}

DSFileWritterBase::DSFileWritterBase(REFCLSID iid)
	: DSFilterBase(iid)
{
	m_bReleaseOnDestroy = true;
}

HRESULT DSFileWritterBase::SetOutputFileName(LPWSTR lpwstrFileName)
{
	if (!m_pFilter) return E_POINTER;
	CComPtr <IFileSinkFilter> pSinkFilter;
	HRESULT hr = m_pFilter->QueryInterface(IID_IFileSinkFilter,(void**)&pSinkFilter);
	SAFECHECK(hr);
	DeleteFileW(lpwstrFileName);
	hr = pSinkFilter->SetFileName(lpwstrFileName, NULL);
	SAFECHECK(hr);
	return S_OK;
}