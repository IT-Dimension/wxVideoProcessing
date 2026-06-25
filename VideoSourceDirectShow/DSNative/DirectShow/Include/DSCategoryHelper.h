#ifndef __DS_CATEGORY_HELPER_H__
#define __DS_CATEGORY_HELPER_H__

#include "DSFilter.h"

// Base category helper class
class DSCategory : public DSObjectBase
{
protected:
	HRESULT GetFiltersCount(long & lCount);
	HRESULT GetFilterName(long lNumber, LPWSTR * plpwstrDevName);
	HRESULT GetDeviceFilter(long lNumber, IBaseFilter ** ppOutFilter);
	HRESULT GetFilterMonikerString(long lNumber, LPWSTR * plpwstrDevMoniker);
protected:
	ICreateDevEnum * m_pDevEnum;
	IEnumMoniker * m_pEnumMoniker;
	REFCLSID m_clsidCategory;
public:
	virtual void Release();
	virtual HRESULT QueryInterface(const IID &riid,void ** ppvObject);
public:
	long get_Count();
	LPWSTR GetName(long lNumber);
	LPWSTR GetDeviceMonikerString(long lNumber);
public:
	virtual DSFilterBase * GetDevice(long lNumber);
public:
	DSCategory(REFCLSID clsidCategory);
	virtual ~DSCategory();
};

// Base video capture category helper class
class DSVideoCaptureCategory: public DSCategory
{
public:
	DSVideoCaptureCategory():DSCategory(CLSID_VideoInputDeviceCategory) {}
	virtual ~DSVideoCaptureCategory() {}
};

// Base audio capture category helper class
class DSAudioCaptureCategory: public DSCategory
{
public:
	DSAudioCaptureCategory():DSCategory(CLSID_AudioInputDeviceCategory) {}
	virtual ~DSAudioCaptureCategory() {}
};

// Base video compressor category helper class
class DSVideoCompressorCategory: public DSCategory
{
public:
	DSVideoCompressorCategory():DSCategory(CLSID_VideoCompressorCategory) {}
	virtual ~DSVideoCompressorCategory() {}
};

// Base audio compressor category helper class
class DSAudioCompressorCategory: public DSCategory
{
public:
	DSAudioCompressorCategory():DSCategory(CLSID_AudioCompressorCategory) {}
	virtual ~DSAudioCompressorCategory() {}
};

#endif
