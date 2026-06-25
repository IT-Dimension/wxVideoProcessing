#include "stdwx.h"
#include "Format.h"
#include <dvdmedia.h>
#include "DirectShow\Include\DSCommon.h"

CVideoFormats::CVideoFormats(IAMStreamConfig * pConfig)
	//: CFactoryObject()
	: m_pConfig(pConfig)
{
	if (m_pConfig)
	{
		m_pConfig->AddRef();
	}
}

CVideoFormats::~CVideoFormats()
{
	SAFERELEASE(m_pConfig);
}

SIZE CVideoFormats::get_Resolution()
{
	SIZE _size;
	_size.cx = 0;
	_size.cy = 0;
	if (m_pConfig)
	{
		AM_MEDIA_TYPE *pmt = NULL;
		if (SUCCEEDED(m_pConfig->GetFormat(&pmt)))
		{
			if (pmt->majortype == MEDIATYPE_Video)
			{
				if (pmt->formattype == FORMAT_VideoInfo)
				{
					VIDEOINFO * pvi = (VIDEOINFO *)pmt->pbFormat;
					_size.cx = pvi->bmiHeader.biWidth;
					_size.cy = pvi->bmiHeader.biHeight;
				}
				else
					if (pmt->formattype == FORMAT_VideoInfo2)
				{
					VIDEOINFOHEADER2 * pvi = (VIDEOINFOHEADER2 *)pmt->pbFormat;
					_size.cx = pvi->bmiHeader.biWidth;
					_size.cy = pvi->bmiHeader.biHeight;
				}
			}
			DeleteMediaType(pmt);
		}
	}
	return _size;
}

int	 CVideoFormats::get_Count()
{
	SIZE _aPrefferedSizes[] = 
	{
		{ 160,120 },
		{ 176,144 },
		{ 240,176 },
		{ 240,180 },
		{ 320,240 },
		{ 352,288 },
		{ 640,240 },
		{ 640,288 },
		{ 640,480 },
		{ 704,576 },
		{ 720,240 },
		{ 720,288 },
		{ 720,480 },
		{ 720,576 },
		{ 800,480 },
		{ 800,600 },
		{ 1024,768 },
		{ 1280,1024 },
		{ 1600,1200 }
	};

	int iSizesCount = sizeof(_aPrefferedSizes)/sizeof(_aPrefferedSizes[0]);
	int iCurrentIndex = 0;
	HRESULT hr = NOERROR;
	bool bResult = false;
	int nIndex = -1;
	if (m_pConfig)
	{
		int iCount, iSize;	
		hr = m_pConfig->GetNumberOfCapabilities(&iCount,&iSize);
		if (hr == S_OK)
		{
			LPBYTE pbtSCC = (LPBYTE)CoTaskMemAlloc(iSize);
			for (int i = 0; i < iCount; i++)
			{
				AM_MEDIA_TYPE * pmt = NULL;
				hr = m_pConfig->GetStreamCaps(i,&pmt,pbtSCC);
				if (hr == S_OK)
				{
					VIDEO_STREAM_CONFIG_CAPS * pCaps = (VIDEO_STREAM_CONFIG_CAPS *)pbtSCC;
					if (pmt->formattype == FORMAT_VideoInfo)
					{
						if (iCurrentIndex == nIndex)
						{
							/*
							VIDEOINFOHEADER * pFormat = (VIDEOINFOHEADER*)pmt->pbFormat;
							_size.cx = pFormat->bmiHeader.biWidth;
							_size.cy = pFormat->bmiHeader.biHeight;
							bResult = true;
							*/
						}
						else
						{
							iCurrentIndex++;
							if (	pCaps->MinOutputSize.cx != pCaps->MaxOutputSize.cx 
									&&	pCaps->MinOutputSize.cy != pCaps->MaxOutputSize.cy)
							{
								// check list of pre-defined formats
								for (int j = 0; j < iSizesCount; j++)
								{								
									// check if supported
									if (
											pCaps->MinOutputSize.cx <= _aPrefferedSizes[j].cx
										&&	pCaps->MaxOutputSize.cx >= _aPrefferedSizes[j].cx
										&&	pCaps->MinOutputSize.cy <= _aPrefferedSizes[j].cy
										&&	pCaps->MaxOutputSize.cy >= _aPrefferedSizes[j].cy
									)
									{
										if (iCurrentIndex == nIndex)
										{
											/*
											_size.cx = _aPrefferedSizes[j].cx;
											_size.cx = _aPrefferedSizes[j].cy;
											bResult = true;
											*/
										}
										else iCurrentIndex++;
									}
									if (bResult) break;
								}
							}
						}
					}
					DeleteMediaType(pmt);
				}
				if (bResult) break;
			}
			CoTaskMemFree(pbtSCC);
		}
	}
	return iCurrentIndex;
}

SIZE CVideoFormats::get_Resolution(int nIndex)
{
	SIZE _aPrefferedSizes[] = 
	{
		{ 160,120 },
		{ 176,144 },
		{ 240,176 },
		{ 240,180 },
		{ 320,240 },
		{ 352,288 },
		{ 640,240 },
		{ 640,288 },
		{ 640,480 },
		{ 704,576 },
		{ 720,240 },
		{ 720,288 },
		{ 720,480 },
		{ 720,576 },
		{ 800,480 },
		{ 800,600 },
		{ 1024,768 },
		{ 1280,1024 },
		{ 1600,1200 }
	};

	int iSizesCount = sizeof(_aPrefferedSizes)/sizeof(_aPrefferedSizes[0]);
	int iCurrentIndex = 0;
	HRESULT hr = NOERROR;
	bool bResult = false;
	SIZE _size;
	_size.cx = 0;
	_size.cy = 0;
	if (m_pConfig)
	{
		int iCount, iSize;	
		hr = m_pConfig->GetNumberOfCapabilities(&iCount,&iSize);
		if (hr == S_OK)
		{
			LPBYTE pbtSCC = (LPBYTE)CoTaskMemAlloc(iSize);
			for (int i = 0; i < iCount; i++)
			{
				AM_MEDIA_TYPE * pmt = NULL;
				hr = m_pConfig->GetStreamCaps(i,&pmt,pbtSCC);
				if (hr == S_OK)
				{
					VIDEO_STREAM_CONFIG_CAPS * pCaps = (VIDEO_STREAM_CONFIG_CAPS *)pbtSCC;
					if (pmt->formattype == FORMAT_VideoInfo)
					{
						if (iCurrentIndex == nIndex)
						{
							VIDEOINFOHEADER * pFormat = (VIDEOINFOHEADER*)pmt->pbFormat;
							_size.cx = pFormat->bmiHeader.biWidth;
							_size.cy = pFormat->bmiHeader.biHeight;
							bResult = true;
						}
						else
						{
							iCurrentIndex++;
							if (	pCaps->MinOutputSize.cx != pCaps->MaxOutputSize.cx 
									&&	pCaps->MinOutputSize.cy != pCaps->MaxOutputSize.cy)
							{
								// check list of pre-defined formats
								for (int j = 0; j < iSizesCount; j++)
								{								
									// check if supported
									if (
											pCaps->MinOutputSize.cx <= _aPrefferedSizes[j].cx
										&&	pCaps->MaxOutputSize.cx >= _aPrefferedSizes[j].cx
										&&	pCaps->MinOutputSize.cy <= _aPrefferedSizes[j].cy
										&&	pCaps->MaxOutputSize.cy >= _aPrefferedSizes[j].cy
									)
									{
										if (iCurrentIndex == nIndex)
										{
											_size.cx = _aPrefferedSizes[j].cx;
											_size.cy = _aPrefferedSizes[j].cy;
											bResult = true;
										}
										else iCurrentIndex++;
									}
									if (bResult) break;
								}
							}
						}
					}
					DeleteMediaType(pmt);
				}
				if (bResult) break;
			}
			CoTaskMemFree(pbtSCC);
		}
	}
	return _size;
}

HRESULT CVideoFormats::set_Resolution(SIZE _size)
{
	return NOERROR;
}