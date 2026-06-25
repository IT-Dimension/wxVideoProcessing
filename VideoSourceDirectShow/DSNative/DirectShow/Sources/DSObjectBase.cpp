
#include "../Include/DSObjectBase.h"
#include <comdef.h>

DSObjectBase::DSObjectBase()
	: m_bReleaseOnDestroy(true)
{
	CoInitialize(NULL);
}

DSObjectBase::~DSObjectBase()
{
	if (m_bReleaseOnDestroy) Release();
}

HRESULT DSObjectBase::QueryInterface(const IID &riid,void ** ppvObject)
{
	return E_NOINTERFACE;
}

void DSObjectBase::Release()
{
}
