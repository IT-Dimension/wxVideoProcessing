#ifndef __Device_H__
#define __Device_H__
/////////////////////////////////////////////////////
//#include "ClassFactory.h"
#include "stdwx.h"
#include "DirectShow/Include/DSCategoryHelper.h"
#include "DirectShow/Include/DSFilter.h"
/////////////////////////////////////////////////////
class CDeviceManager;
class CFrameGrabber;
class CVideoFormats;
/////////////////////////////////////////////////////
class EXPORTS_API CDevice //: public CFactoryObject
{
	friend class CDeviceManager;
	friend class CFrameGrabber;
private:
	LPWSTR m_lpwstrMoniker;
	DSFilterBase * m_pFilter;
private:
	CDevice(DSFilterBase * pFilter);
public:
	LPWSTR get_Moniker();
	void set_Moniker(LPCWSTR lpwstrMoniker);
	CFrameGrabber * CreateFrameGrabber();
	CVideoFormats * get_VideoFormats();
public:
	HRESULT ShowProperties(HWND hwndParent = NULL);
	BOOL HaveProperties();
public:
	CDevice(LPOLESTR szMoniker);
	CDevice(REFCLSID iid);
	virtual ~CDevice();
};
/////////////////////////////////////////////////////
#endif //__Device_H__