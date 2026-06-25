#ifndef __Formats_H__
#define __Formats_H__
/////////////////////////////////////////////////////
//#include "ClassFactory.h"
#include "stdwx.h"
#include <streams.h>
/////////////////////////////////////////////////////
class EXPORTS_API CVideoFormats //: public CFactoryObject
{
	friend class CDevice;
private:
	IAMStreamConfig * m_pConfig;
public:
	SIZE get_Resolution();
	int	 get_Count();
	SIZE get_Resolution(int nIndex);
	HRESULT set_Resolution(SIZE _size);
private:
	CVideoFormats(IAMStreamConfig * pConfig);
public:
	virtual ~CVideoFormats();
};
/////////////////////////////////////////////////////
#endif // __Formats_H__