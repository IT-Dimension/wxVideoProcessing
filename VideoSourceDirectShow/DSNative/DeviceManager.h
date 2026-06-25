#ifndef __DeviceManager_H__
#define __DeviceManager_H__
/////////////////////////////////////////////////////
//#include "ClassFactory.h"
#include "Device.h"
#include "DirectShow/Include/DSCategoryHelper.h"
/////////////////////////////////////////////////////
class EXPORTS_API CDeviceManager//: public CFactoryObject
{
private:
	DSCategory * m_pCategory;
	REFCLSID m_clsidCategory;
public:
	static CDevice * get_DeviceByMoniker(LPCWSTR lpwstrMonikerName);
public:
	long get_Count();
	CDevice * get_Device(LPCWSTR lpwstrName);
	CDevice * get_Device(const long lIndex);
	LPWSTR get_DeviceName(const long lIndex);
	LPWSTR get_DeviceMonikerString(const long lIndex);
	long get_DeviceIndex(LPCWSTR lpwstrMonikerName);

	void RefreshDevices();
public:
	CDeviceManager(REFCLSID _category);
	virtual ~CDeviceManager();
};
/////////////////////////////////////////////////////
#endif // __DeviceManager_H__