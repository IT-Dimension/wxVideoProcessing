#include "stdwx.h"
#include "DeviceManager.h"
/////////////////////////////////////////////////////
CDeviceManager::CDeviceManager(REFCLSID _category)
//: CFactoryObject()
: m_pCategory(NULL)
, m_clsidCategory(_category)
{
	m_pCategory = new DSCategory(m_clsidCategory);
}

CDeviceManager::~CDeviceManager()
{
	SAFEDELETE(m_pCategory);
}

/////////////////////////////////////////////////////
long CDeviceManager::get_Count()
{
	if (m_pCategory)
	{
		return m_pCategory->get_Count();
	}
	return 0;
}

/////////////////////////////////////////////////////
CDevice * CDeviceManager::get_DeviceByMoniker(LPCWSTR lpwstrMonikerName)
{
	CDevice * pDevice = new CDevice((LPOLESTR)lpwstrMonikerName);
	if (pDevice->m_pFilter)
	{
		return pDevice;
	}
	else
	{
		delete pDevice;
	}
	return NULL;
}

/////////////////////////////////////////////////////
LPWSTR CDeviceManager::get_DeviceName(const long lIndex)
{
	if (m_pCategory)
	{
		long lNumber = lIndex;
		return m_pCategory->GetName(lNumber);
	}
	return NULL;
}

LPWSTR CDeviceManager::get_DeviceMonikerString(const long lIndex)
{
	if (m_pCategory)
	{
		long lNumber = lIndex;
		return m_pCategory->GetDeviceMonikerString(lNumber);
	}
	return NULL;
}
/////////////////////////////////////////////////////
CDevice * CDeviceManager::get_Device(const long lIndex)
{
	if (m_pCategory)
	{
		DSFilterBase * pFilter = m_pCategory->GetDevice(lIndex);
		if (pFilter)
		{
			CDevice * pDevice = new CDevice(pFilter);
			if (pDevice)
			{
				LPWSTR lpwstrMoniker = get_DeviceMonikerString(lIndex);
				if (lpwstrMoniker)
				{
					pDevice->set_Moniker(lpwstrMoniker);
					SAFEDELETEARRAY(lpwstrMoniker);
				}
				else
				{
					delete pDevice;
					pDevice = NULL;
				}
			}
			return pDevice;
		}
	}
	return NULL;
}

CDevice * CDeviceManager::get_Device(LPCWSTR lpwstrName)
{
	if (m_pCategory)
	{
		long lCount = m_pCategory->get_Count();
		while (lCount > 0)
		{
			lCount--;
			LPWSTR lpwstrDevice = m_pCategory->GetName(lCount);
			bool found = wcscmp(lpwstrDevice, lpwstrName) == 0;
			SAFEDELETEARRAY(lpwstrDevice);
			if (found)
				return get_Device(lCount);
		}
	}
	return NULL;
}

long CDeviceManager::get_DeviceIndex(LPCWSTR lpwstrMonikerName)
{
	if (m_pCategory)
	{
		long lCount = m_pCategory->get_Count();
		while (lCount > 0)
		{
			lCount--;
			LPWSTR lpwstrDevice = get_DeviceMonikerString(lCount);
			bool found = wcscmp(lpwstrDevice, lpwstrMonikerName) == 0;
			SAFEDELETEARRAY(lpwstrDevice);
			if (found)
				return lCount;
		}
	}
	return -1;
}

void CDeviceManager::RefreshDevices()
{
	SAFEDELETE(m_pCategory);
	m_pCategory = new DSCategory(m_clsidCategory);
}
/////////////////////////////////////////////////////