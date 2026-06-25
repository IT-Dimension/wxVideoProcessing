#include "stdwx.h"
#include "VideoGrabberDirectShow.h"
#include "Crossbar.h"

VideoGrabberDirectShow::VideoGrabberDirectShow()
: m_DeviceIndex(0)
, m_DeviceManager(CLSID_VideoInputDeviceCategory)
, m_pDevice(nullptr)
, m_pGrabber(nullptr)
, m_pbmi(nullptr)
, m_pBuffer(nullptr)
{
}

VideoGrabberDirectShow::VideoGrabberDirectShow(VideoSourcePluginBase * owner, size_t index)
: VideoGrabberBase(owner)
, m_DeviceIndex((int)index)
, m_DeviceManager(CLSID_VideoInputDeviceCategory)
, m_pDevice(nullptr)
, m_pGrabber(nullptr)
, m_pbmi(nullptr)
, m_pBuffer(nullptr)
{
	m_ImageSize = wxDefaultSize;
	m_pDevice = m_DeviceManager.get_Device(m_DeviceIndex);
	if (m_pDevice)
	{
		m_DeviceID = m_pDevice->get_Moniker();
		ReadName();
	}
	StartGrabber();
}

VideoGrabberDirectShow::VideoGrabberDirectShow(VideoSourcePluginBase * owner, const wxString & id)
: VideoGrabberBase(owner)
, m_DeviceManager(CLSID_VideoInputDeviceCategory)
, m_pDevice(nullptr)
, m_pGrabber(nullptr)
, m_DeviceID(id)
, m_pbmi(nullptr)
, m_pBuffer(nullptr)
{
	m_ImageSize = wxDefaultSize;
	m_pDevice = m_DeviceManager.get_DeviceByMoniker(m_DeviceID.wc_str());
	m_DeviceIndex = m_DeviceManager.get_DeviceIndex(m_DeviceID.wc_str());
	if (m_pDevice)
		ReadName();
	StartGrabber();
}
VideoGrabberDirectShow::~VideoGrabberDirectShow()
{
	CAutoLock lock(&m_csLock);
	StopGrabber();
	wxDELETE(m_pDevice);
}

wxString VideoGrabberDirectShow::GetName() const
{
	return m_DeviceName;
}

wxString VideoGrabberDirectShow::GetID() const
{
	return m_DeviceID;
}

size_t VideoGrabberDirectShow::GetFrameDataLength()
{
	return m_ImageSize.GetWidth() * m_ImageSize.GetHeight() * 3;
}

wxSize VideoGrabberDirectShow::GetFrameSize()
{
	return m_ImageSize;
}

size_t VideoGrabberDirectShow::get_ResolutionsCount()
{
	size_t _result = 0;
	if (m_pDevice)
	{
		CVideoFormats * pFormats = m_pDevice->get_VideoFormats();
		if (pFormats)
		{
			_result = pFormats->get_Count();
			delete pFormats;
			pFormats = nullptr;
		}
	}
	return _result;
}

wxSize VideoGrabberDirectShow::get_ResolutionByIndex(size_t nIndex)
{
	wxSize _result = m_ImageSize;
	if (m_pDevice)
	{
		CVideoFormats * pFormats = m_pDevice->get_VideoFormats();
		if (pFormats)
		{
			SIZE _size = pFormats->get_Resolution((int)nIndex);
			_result.x = _size.cx;
			_result.y = _size.cy;
			delete pFormats;
			pFormats = nullptr;
		}
	}
	return _result;
}

bool VideoGrabberDirectShow::set_Resolution(wxSize _size)
{
	bool bResult = false;
	if (m_pDevice)
	{
		CAutoLock lock(&m_csLock);
		StopGrabber();
		CVideoFormats * pFormats = m_pDevice->get_VideoFormats();
		if (pFormats)
		{
			SIZE _SizeToSet;
			_SizeToSet.cx = _size.x;
			_SizeToSet.cy = _size.y;
			bResult = SUCCEEDED(pFormats->set_Resolution(_SizeToSet));
			delete pFormats;
			pFormats = nullptr;
		}
		StartGrabber();
	}
	return bResult;
}

bool VideoGrabberDirectShow::set_Resolution(size_t nIndex)
{
	return set_Resolution(get_ResolutionByIndex(nIndex));
}

unsigned char * VideoGrabberDirectShow::GetBuffer()
{
	return m_pBuffer;
}

unsigned char * VideoGrabberDirectShow::GrabFrame(bool bUseInternalBuffer)
{
	unsigned char * pBuffer = nullptr;
	long lLength = 0;
	if (bUseInternalBuffer)
	{
		lLength = GetFrameDataLength();
		pBuffer = m_pBuffer;
	}
	if (GrabFrame(&pBuffer,&lLength))
	{
		return pBuffer;
	}
	else
	{
		if (!bUseInternalBuffer)
		{
			if (pBuffer) CoTaskMemFree(pBuffer);
			pBuffer = nullptr;
		}
	}
	return nullptr;
}

int VideoGrabberDirectShow::GrabFrame(unsigned char ** data, long * length)
{
	CAutoLock lock(&m_csLock);
	if (m_pGrabber)
	{
		try
		{
			return m_pGrabber->GrabFrame(data,length);
		}
		catch (...)
		{
			DbgOutString(_T("Error while perform grab into passed buffer"));
		}
	}
	return 0;
}

bool VideoGrabberDirectShow::IsOK()
{
	return m_pGrabber != nullptr;
}

bool VideoGrabberDirectShow::StartGrabber()
{
	if (m_pDevice)
	{
		CAutoLock lock(&m_csLock);
		if(m_pGrabber) StopGrabber();
		m_pGrabber = m_pDevice->CreateFrameGrabber();
		if (m_pGrabber)
		{
			m_pGrabber->set_ShouldPreview(false);
			if (SUCCEEDED(m_pGrabber->Start()))
			{
				if (S_OK == m_pGrabber->get_Format(&m_pbmi))
				{
					m_ImageSize.x = m_pbmi->biWidth;
					m_ImageSize.y = m_pbmi->biHeight;
				}
				m_pBuffer = (LPBYTE)CoTaskMemAlloc(GetFrameDataLength());
				/*
				CCrossBar * pCrossBar = m_pGrabber->get_Crossbar();
				if (pCrossBar)
				{
					long lIndex = pCrossBar->get_InputByType(PhysConn_Video_Composite);
					//long lIndex = pCrossBar->get_InputByType(PhysConn_Video_Tuner);
					if (lIndex != -1)
					{
						pCrossBar->Route(lIndex);
					}
					delete pCrossBar;
				}
				*/
				return true;
			}
			else
			{
				StopGrabber();
			}
		}
	}
	return false;
}
bool VideoGrabberDirectShow::StopGrabber()
{
	CAutoLock lock(&m_csLock);
	wxDELETE(m_pGrabber);
	
	if (m_pbmi != nullptr)
	{
		CoTaskMemFree(m_pbmi);
		m_pbmi = nullptr;
	}
	if (m_pBuffer)
	{
		CoTaskMemFree(m_pBuffer);
		m_pBuffer = nullptr;
	}
	return true;
}

void VideoGrabberDirectShow::ReadName()
{
	LPWSTR lpwstrName = m_DeviceManager.get_DeviceName(m_DeviceIndex);
	m_DeviceName = lpwstrName;
	wxDELETEA(lpwstrName);
}