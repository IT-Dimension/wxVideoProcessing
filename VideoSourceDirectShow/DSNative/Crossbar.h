#ifndef __Crossbar_H__
#define __Crossbar_H__
/////////////////////////////////////////////////////
//#include "ClassFactory.h"
#include "stdwx.h"
#include <streams.h>
/////////////////////////////////////////////////////
class EXPORTS_API CCrossBar //: public CFactoryObject
{
	friend class CFrameGrabber;
private:
	IAMCrossbar * m_pCrossbar;
private:
	BOOL IsVideo(long lPin,BOOL bOutput);
public:
	long get_Count(BOOL bVideo = TRUE);
	long get_Active(BOOL bVideo = TRUE);
	long get_InputByType(PhysicalConnectorType _type);
	long get_OutputByType(PhysicalConnectorType _type);
	LPWSTR get_PinName(long lIndex,BOOL bVideo = TRUE);
	HRESULT Route(long lIndex,BOOL bVideo = TRUE);
private:
	CCrossBar(IAMCrossbar * pCrossbar);
public:
	virtual ~CCrossBar();
};
/////////////////////////////////////////////////////
#endif // __Crossbar_H__