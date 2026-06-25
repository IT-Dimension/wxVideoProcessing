//////////////////////////////////////////////////////////
// Helper macro definitions
// Created by Sonic
// for any additional features contact sonic@YoooMedia.com
/////////////////////////////////////////////////////////

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __YoooCommonDefs_H__
#define __YoooCommonDefs_H__

// Safe releasing COM object
#define SAFERELEASE(comobj) \
if (comobj)					\
{							\
	comobj->Release();		\
	comobj		= NULL;		\
}							\

// Deleting object
#define SAFEDELETE(obj)		\
if (obj)					\
{							\
	delete obj;				\
	obj = NULL;				\
}

// Free memory
#define SAFECOTASKFREE(obj)		\
if (obj)						\
{								\
	CoTaskMemFree((LPVOID)obj);	\
	obj = NULL;					\
}

// delete an array
#define SAFEDELETEARRAY(obj)	\
if (obj)					\
{							\
	delete[] obj;			\
	obj = NULL;				\
}

// Safe Check HRESULT value
#define SAFECHECK(hr)		\
ASSERT( S_OK == hr );		\
if (FAILED(hr))				\
{							\
	return hr;				\
}

// Safe Check HRESULT value and close interfaces
#define SAFECHECKCLOSE(hr)	\
ASSERT( S_OK == hr );		\
if (FAILED(hr))				\
{							\
	CloseInterfaces();		\
	return hr;				\
}

#define CHECKRET(object,ret)	\
if (!object) return ret

#define CHECKRETBOOL(object)	\
CHECKRET(object,false)

#define CHECKRETHR(object)	\
CHECKRET(object,E_POINTER)

#define THROWHR(hr)			\
if (hr != S_OK) throw hr;

#define SAFERELEASEDC(hdc,hwnd)		\
if (hdc)							\
{									\
	ReleaseDC(hwnd,hdc);			\
	hdc = NULL;						\
}

#define DELETEGDIOBJECT(obj)		\
if (obj)							\
{									\
	DeleteObject(obj);				\
	obj = NULL;						\
}

#define DELETEGDIDC(hdc)			\
if (hdc)							\
{									\
	DeleteDC(hdc);					\
	hdc = NULL;						\
}

#endif // __YoooCommonDefs_H__