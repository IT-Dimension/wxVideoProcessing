#include "stdwx.h"
#include "Crossbar.h"
#include "DirectShow\Include\DSCommon.h"

CCrossBar::CCrossBar(IAMCrossbar * pCrossbar)
	//: CFactoryObject()
	: m_pCrossbar(pCrossbar)
{
	if (m_pCrossbar)
	{
		m_pCrossbar->AddRef();
	}
}

CCrossBar::~CCrossBar()
{
	SAFERELEASE(m_pCrossbar);
}

long CCrossBar::get_Count(BOOL bVideo)
{
	long lCount = 0;
	if (m_pCrossbar)
	{
		long lInputs	= 0;
		long lOutputs	= 0;
		m_pCrossbar->get_PinCounts(&lOutputs,&lInputs);
		if (lInputs > 0)
		{
			for (long i = 0; i < lInputs; i++)
			{
				if (IsVideo(i,FALSE))
				{
					lCount++;
				}
			}
		}

	}
	return lCount;
}

long CCrossBar::get_Active(BOOL bVideo)
{
	long lInput		= -1;
	if (m_pCrossbar)
	{
		long lInputs	= 0;
		long lOutputs	= 0;
		m_pCrossbar->get_PinCounts(&lOutputs,&lInputs);
		long lPin = 0;
		while (lPin < lOutputs)
		{
			if (IsVideo(lPin,TRUE))
			{
				if (bVideo) break;
			}
			else
			{
				if (!bVideo) break;
			}
			lPin++;
		}
		m_pCrossbar->get_IsRoutedTo(lPin,&lInput);
		long lCount = get_Count(bVideo);
		if (lInput >= lCount)
		{
			lInput -= get_Count(!bVideo);
		}
	}
	return lInput;
}

LPWSTR CCrossBar::get_PinName(long lIndex,BOOL bVideo)
{
	if (m_pCrossbar)
	{
		long lRelatedIndex;
		long lPhysicalType;
		if (IsVideo(lIndex,FALSE))
		{
			if (!bVideo) lIndex += get_Count(bVideo);
		}
		else
		{
			if (bVideo) lIndex += get_Count(!bVideo);
		}

		if (S_OK == m_pCrossbar->get_CrossbarPinInfo(TRUE,lIndex,&lRelatedIndex,&lPhysicalType))
		{
			switch (lPhysicalType)
			{
			case PhysConn_Video_Tuner:
				return L"Video Tuner";
			case PhysConn_Video_Composite:
				return L"Video Composite";
			case PhysConn_Video_SVideo:
				return L"Video S-Video";
			case PhysConn_Video_RGB:
				return L"Video RGB";
			case PhysConn_Video_YRYBY:
				return L"Video YRYBY";
			case PhysConn_Video_SerialDigital:
				return L"Video SerialDigital";
			case PhysConn_Video_ParallelDigital:
				return L"Video ParallelDigital";
			case PhysConn_Video_SCSI:
				return L"Video SCSI";
			case PhysConn_Video_AUX:
				return L"Video AUX";
			case PhysConn_Video_1394:
				return L"Video 1394";
			case PhysConn_Video_USB:
				return L"Video USB";
			case PhysConn_Video_VideoDecoder:
				return L"Video VideoDecoder";
			case PhysConn_Video_VideoEncoder:
				return L"Video VideoEncoder";
			case PhysConn_Video_SCART:
				return L"Video SCART";
			case PhysConn_Audio_Tuner:
				return L"Audio Tuner";
			case PhysConn_Audio_Line:
				return L"Audio Line";
			case PhysConn_Audio_Mic:
				return L"Audio Mic";
			case PhysConn_Audio_AESDigital:
				return L"Audio AESDigital";
			case PhysConn_Audio_SPDIFDigital:
				return L"Audio SPDIFDigital";
			case PhysConn_Audio_SCSI:
				return L"Audio SCSI";
			case PhysConn_Audio_AUX:
				return L"Audio AUX";
			case PhysConn_Audio_1394:
				return L"Audio 1394";
			case PhysConn_Audio_USB:
				return L"Audio USB";
			case PhysConn_Audio_AudioDecoder:
				return L"Audio AudioDecoder";
			}
		}
	}
	return NULL;
}

HRESULT CCrossBar::Route(long lIndex,BOOL bVideo)
{
	if (!m_pCrossbar) return E_FAIL;
	if (IsVideo(lIndex,FALSE))
	{
		if (!bVideo) lIndex += get_Count(bVideo);
	}
	else
	{
		if (bVideo) lIndex += get_Count(!bVideo);
	}
	long lInputs	= 0;
	long lOutputs	= 0;
	m_pCrossbar->get_PinCounts(&lOutputs,&lInputs);
	long lPin = 0;
	while (lPin < lOutputs)
	{
		if (IsVideo(lPin,TRUE))
		{
			if (bVideo) break;
		}
		else
		{
			if (!bVideo) break;
		}
		lPin++;
	}
	return m_pCrossbar->Route(lPin,lIndex);
}

BOOL CCrossBar::IsVideo(long lPin,BOOL bOutput)
{
	if (m_pCrossbar)
	{
		long lRelatedIndex;
		long lPhysicalType;
		if (S_OK == m_pCrossbar->get_CrossbarPinInfo(!bOutput,lPin,&lRelatedIndex,&lPhysicalType))
		{
			return (lPhysicalType < PhysConn_Audio_Tuner);
		}
	}
	return FALSE;
}

long CCrossBar::get_InputByType(PhysicalConnectorType _type)
{
	if (m_pCrossbar)
	{
		long lInputs	= 0;
		long lOutputs	= 0;
		m_pCrossbar->get_PinCounts(&lOutputs,&lInputs);
		if (lInputs > 0)
		{
			for (long i = 0; i < lInputs; i++)
			{
				long lRelatedIndex;
				long lPhysicalType;
				if (S_OK == m_pCrossbar->get_CrossbarPinInfo(TRUE,i,&lRelatedIndex,&lPhysicalType))
				{
					if (lPhysicalType == _type) return i;
				}
			}
		}
	}
	return -1;
}

long CCrossBar::get_OutputByType(PhysicalConnectorType _type)
{
	if (m_pCrossbar)
	{
		long lInputs	= 0;
		long lOutputs	= 0;
		m_pCrossbar->get_PinCounts(&lOutputs,&lInputs);
		if (lOutputs > 0)
		{
			for (long i = 0; i < lOutputs; i++)
			{
				long lRelatedIndex;
				long lPhysicalType;
				if (S_OK == m_pCrossbar->get_CrossbarPinInfo(FALSE,i,&lRelatedIndex,&lPhysicalType))
				{
					if (lPhysicalType == _type) return i;
				}
			}
		}
	}
	return -1;
}