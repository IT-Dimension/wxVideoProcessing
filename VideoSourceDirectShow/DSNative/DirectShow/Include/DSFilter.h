
#ifndef __DS_FILTER_HELPER_H__
#define __DS_FILTER_HELPER_H__

#include "DSObjectBase.h"

class DSFilterBase : public virtual DSObjectBase
{
protected:
	IBaseFilter * m_pFilter;
protected:
	HRESULT GetFilterByMoniker(LPOLESTR szMoniker);
public:
	virtual void Release();
	virtual HRESULT QueryInterface(const IID &riid,void ** ppvObject);
public:
	bool GetIBaseFilter(IBaseFilter ** pIBaseFilter);
	HRESULT AddToFilterGraph(IGraphBuilder * pGraphBuilder);
	HRESULT RemoveFromFilterGraph(IGraphBuilder * pGraphBuilder);
public:
	HRESULT ShowProperties(HWND hwndParent = NULL);
	bool HaveProperties();
	IPin * GetPinByDirection(PIN_DIRECTION direction,long lPinNumber);
	IPin * GetPinByCategory(REFGUID guidPinCategory);

	HRESULT GetOutputPinCapabilitiesCount(long lPinNumber, int *piCount);
	HRESULT GetOutputPinCapabilitiesCount(IPin * pPin, int *piCount);
	HRESULT GetOutputPinCapabilitiesCount(REFGUID guidPinCategory, int *piCount);

	HRESULT GetOutputPinCapabilities(REFGUID guidPinCategory, int iCapNumber, AM_MEDIA_TYPE ** ppmt);
	HRESULT GetOutputPinCapabilities(long lPinNumber, int iCapNumber, AM_MEDIA_TYPE ** ppmt);
	HRESULT GetOutputPinCapabilities(IPin * pPin, int iCapNumber, AM_MEDIA_TYPE ** ppmt);

	HRESULT GetOutputPinFormat(long lPinNumber, AM_MEDIA_TYPE ** ppmt);
	HRESULT GetOutputPinFormat(IPin * pPin, AM_MEDIA_TYPE ** ppmt);
	HRESULT GetOutputPinFormat(REFGUID guidPinCategory, AM_MEDIA_TYPE ** ppmt);

	HRESULT SetOutputPinFormat(long lPinNumber, AM_MEDIA_TYPE * pmt);
	HRESULT SetOutputPinFormat(IPin * pPin, AM_MEDIA_TYPE * pmt);
	HRESULT SetOutputPinFormat(REFGUID guidPinCategory, AM_MEDIA_TYPE * pmt);

protected:
	DSFilterBase();
public:
	DSFilterBase(LPOLESTR szMoniker);
	DSFilterBase(REFCLSID iid);
	virtual ~DSFilterBase();
public:
	DSFilterBase(IBaseFilter * pFilter, bool bReleaseOnDestroy = false);
};

#endif
