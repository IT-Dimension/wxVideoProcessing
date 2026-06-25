// IDMIPC.h: interface for the CIDMIPC class.
//
//////////////////////////////////////////////////////////////////////

/** @mainpage
 *  
 * \section intro Introduction
 *
 * Use this SDK to communicate with Geovision multicam system
 *
 * \section start Starting
 * include "IDMIPCClient.h" and 
 *  add DMIPC.lib to your project
 *
 * \section usage Usage
 *
 * 1.Dynamic or static link this libary \n
 * 2.Call CreateIDMIPC to get a CIDMIPC instance then cast it to CIDMIPCClient \n
 * 3.Call CIDMIPCClient.Initialize() \n
 * 4.Implement CIDMIPCClientCallBack class \n
 * 5.Call CIDMIPCClient.RegClientCallBack and register the CIDMIPCClientCallBack object \n
 * 6.Start programming \n
 *
 *		see sample code for more detail
 *
 * \section test Test Your program
 * After bulid your project ,
 * copy the output .exe file to the same folder of Geovision gv*.exe(depend on your version) and dmipc.dll\n
 * Note* Run gv*.exe before run your program 
 *
 * \section History
 *	
 *	1.2.2.0		Add a new export function CreateIDMIPCClient\n
 *              Fix some bugs \n
 *              Release with multicam v6.1.0.0
 *
 *	1.2.1.0		release for SDK
 *
 *
 *
 *	2.0.0.0		major update for YUV format /remote control etc.
 *
 *
 *	2.0.0.1		Fix change image size crash bug
 *
 *	3.1.0.0		Support VS_PTZ
 *
 *	4.0.0.0		Add single image for POS/Obj triggers	/ instant playback
 *
 *	4.1.0.0		Add IP/ID information for some commands
 *				Add Non-Stop PTZ commands
 *	4.1.0.1		Add new command for preset (up to 64)
 *				
 *  5.0.0.0		Add new function for mega-pixel video
 */

// Define this file _IDMIPC_H_
#ifndef _IDMIPC_H_
#define _IDMIPC_H_

#include "IPC_define.h"
#include "IPC_struct.h"

#ifdef _DMIPCDLL
#define DMIPCDLLAPI extern "C" __declspec(dllexport)
#else
#define DMIPCDLLAPI extern "C" __declspec(dllimport)
#endif


class CIDMIPC;

typedef CIDMIPC* (__stdcall *CreateIDMIPCFunction)(IDMIPCInitData& InitData);
typedef void (__stdcall *ReleaseIDMIPCFunction)(CIDMIPC*);
typedef unsigned long (__stdcall *GetDMIPCVersionFunctin)();
typedef bool (__stdcall *CheckServerHealthFunction)();
typedef int (__stdcall *CheckServerHealthExFunction)();

/** @defgroup Export DLL export functions
 *  
 *  @{
 */

/** 
	*Use this function to get a CIDMIPC instance,
	*then cast it to CIDMIPCClient (MS VC++ user can use dynamic_cast for safety) \n
	*After get CIDMIPCClient ,call Initialize() and RegClientCallBack(CIDMIPCClientCallBack*)
	*so that can starting communacation with multicam server
	*@param InitData init struct
*/
DMIPCDLLAPI CIDMIPC* __stdcall CreateIDMIPC(IDMIPCInitData& InitData); 
/** 
	*Use this function to release resource created by CreateIDMIPC 
	
*/
DMIPCDLLAPI void __stdcall ReleaseIDMIPC(CIDMIPC*);

/**
	*Get the version number of current DMIPC.dll
*/
DMIPCDLLAPI unsigned long __stdcall GetDMIPCVersion();

/**
	*Check if server alive
*/
DMIPCDLLAPI bool __stdcall CheckServerHealth();

/**
	*Check if server alive for more information (this may block 1min if server down)
*/
DMIPCDLLAPI int __stdcall CheckServerHealthEx();

/**
	*Cancel CheckServerHealthEx
*/
DMIPCDLLAPI void  __stdcall CancelCheckServerHealth();	
/** @} */ // end of Export

 /**
     * Base class of CallBack classes
     *
     */ 
class CIDMIPCCallBack{
protected:
	CIDMIPCCallBack(){};
	virtual ~CIDMIPCCallBack(){};
};


 /**
     * Base class of DMIPC interface
     *
     */ 
class CIDMIPC  
{
public:
	/**
     * Initialize this object,must called before all operations
     *
     */ 
	virtual bool __stdcall Initialize()=0;
	/**
     * protoltype of Regester call back functions 
     * call derived regester function to regester call back object
     */ 
	virtual bool __stdcall RegCallBack(CIDMIPCCallBack* pCallBack)=0;
	/**
     * UnRegister call back object
     * 
     */ 
	virtual void __stdcall UnRegCallBack()=0;

protected:
	~CIDMIPC(){};	
};

#endif // _IDMIPC_H_
