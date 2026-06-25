
#ifndef __DS_OBJECT_BASE_H__
#define __DS_OBJECT_BASE_H__

#include <stdwx.h>
#include "DSCommon.h"
#include <streams.h>

class DSObjectBase
{
protected:
	bool m_bReleaseOnDestroy;
public:
	bool get_ReleaseOnDestroy();
	void set_ReleaseOnDestroy(bool bReleaseOnDestroy);
public:
	virtual void Release();
	virtual HRESULT QueryInterface(const IID &riid,void ** ppvObject);
protected:
	DSObjectBase();
public:
	virtual ~DSObjectBase();
};

#endif