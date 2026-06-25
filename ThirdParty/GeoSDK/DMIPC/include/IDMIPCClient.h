// IDMIPCClient.h: interface for the CIDMIPCClient class.
//
//////////////////////////////////////////////////////////////////////
/*

	4.0.0.0		Add single image for Obj/Pos event

	3.0.0.0		Redefine IPC_REQ_IO/IPC_TRIGGER_IO

	1.2.4.0		Add PTZ dome data query

	1.2.3.1		Add yuy2 image support

	1.2.3.0		Add version control

	1.2.2.0		Add a new export function CreateIDMIPCClient
				fix a bug that bSucceed in SendCommand function always FALSE
				nCam in IPC_REQ_PTZ now works fine with multicam's cammapping setting
				Add a new command DMIPC_PARA_RCL_DATE
				Add two new triggers DMIPC_PARA_REC and DMIPC_PARA_RECYCLE
				

	1.2.1.0		release for SDK

*/


/**
* Interface of DMIPC call back functions
* Client must implement this class and register it to DMIPC object 

	*/

// Define this file _IDMIPCCLIENT_H_
#ifndef _IDMIPCCLIENT_H_
#define _IDMIPCCLIENT_H_

#include "IDMIPC.h"

class CIDMIPCClient;
class CIDMIPCClientEx;

typedef CIDMIPCClient* (__stdcall *CreateIDMIPCClientFunction)(IDMIPCInitData& InitData);
typedef CIDMIPCClientEx* (__stdcall *CreateIDMIPCClientExFunction)(IDMIPCInitData& InitData);

DMIPCDLLAPI CIDMIPCClient* __stdcall CreateIDMIPCClient(IDMIPCInitData& InitData); 
DMIPCDLLAPI CIDMIPCClientEx* __stdcall CreateIDMIPCClientEx(IDMIPCInitData& InitData); 

class CIDMIPCClientCallBack : public CIDMIPCCallBack{
public:
	 /**
     * called when multicam system replies client's command
     *
     * @param pRequest replied header
     * @param pData replied data(if necessary)
     *
     */ 
	virtual void __stdcall OnReply(LPDMRequest pRequest,LPDMData pData)=0;
	/**
     * called when multicam system replies client's request of video data
     *
     * @param iCam camera index(one base,see IPC_define.h for detail)
     * @param pHeader Header
     * @param pData image data
	 *
     */ 
	virtual void __stdcall OnRpyVideo(UINT iCam,IPC_RPY_CAM* pHeader,LPVOID pData)=0;
	/**
     * called when multicam system replies client's request of audio data
     *
     * @param iCam camera index(one base,see IPC_define.h for detail)
     * @param pHeader Header
     * @param pData sound data
	 *
     */ 
	virtual void __stdcall OnRpyAudio(UINT iCam,IPC_RPY_AUDIO* pHeader,LPVOID pData)=0;
	/**
     * called when multicam system replies client's request of Trigger data
     *
     * @param pHeader Header
     * @param pData  data
	 *
     */ 
	virtual void __stdcall OnRpyTrigger(LPDMRequest pHeader,LPDMData pData)=0;
	/**
     * called when multicam system shut down service
     * not implement yet
	 *
     */ 
	virtual void __stdcall OnServerStop(){};
};
/**
* Extended Interface of DMIPC call back functions
* inheritance from CIDMIPCClientCallBack

	*/

class CIDMIPCClientCallBackExt : public CIDMIPCClientCallBack{
public:
	/**
     * called when multicam system restart service,
	 * not implement yet
	 *
     */ 
	virtual void __stdcall OnServerRestart(){};
};

/**
* Interface of DMIPC 
* Client use this object to communacate this multicam server
	*/

class CIDMIPCClient : public CIDMIPC  
{
protected:
	/**
     * destroctor, DO NOT and CANNOT free this object by operator "delete" 
     *
     */ 
	virtual ~CIDMIPCClient();
public:
	/**
     * register the call back functions
     * @param pCallBack the call back object pointer
     */ 
	virtual bool __stdcall RegClientCallBack(CIDMIPCClientCallBack* pCallBack){return false;};
	
	/**
		*	Send request to multicam system 
		* @param pDMRequest Request header
		*
		* @param pDMData	Request data,struct inheritance from DMData and depend on pDMRequest->message and pDMRequest->para
		
	*/
	virtual bool __stdcall Request(const LPDMRequest& pDMRequest,const LPDMData& pDMData){return false;};
	/**
		*	
		*	same as Request but it is synchronized
		* @param pDMRequestOut Replied header
		* @param pDMDataOut	Replied data
		*
		
	*/
	virtual bool __stdcall SyncRequest(const LPDMRequest& pDMRequestIn,const LPDMData& pDMDataIn,LPDMRequest& pDMRequestOut,LPDMData& pDMDataOut){return false;};

	/**
		*do DMIPC_MSG_REQUEST_CAM message request (request for video frame)
		* @param uCam Camera index
		* @param bSync Is synchronized request?
		* @param pRpyCam If bSync is true ,the reply data will filled here(header data)
		* @param pData same as pRpyCam(image data)
	*/
	virtual bool __stdcall GrabFrame(UINT uCam,bool bSync=false,IPC_RPY_CAM** pRpyCam=NULL,LPVOID* pData=NULL){return false;};
	/**
		*do DMIPC_MSG_REQUEST_CMD message request (request for command)
		* @param uCmd Command id
		* @param pDMData Command data(detail) if nessecary
		* @param bSync Is synchronized request?
		* @param pbSucceed Command succeed?
		* @param pData Replied data (if nessecary)
	*/
	virtual bool __stdcall SendCommand(UINT uCmd,const LPDMData& pDMData,bool bSync=false,BOOL* pbSucceed=NULL,LPDMData* pData=NULL){return false;};
	/**
		*do DMIPC_MSG_REQUEST_AUDIO message request(request for audio data)
		* @param uCam Camera index
		* @param bSync Is synchronized request?
		* @param pRpyCam If bSync is true ,the reply data will filled here(header data)
		* @param pData Replied data (audio data)
	*/

	virtual bool __stdcall ReqAudio(UINT uCam,bool bSync=false,IPC_RPY_AUDIO** pRpyCam=NULL,LPVOID* pData=NULL){return false;};
	/**
		*do DMIPC_MSG_REQUEST_TRIGGER message request(request for event trigger)
		* @param pData Request detail
		*
	*/
	virtual bool __stdcall ReqTrigger(IPC_REQ_TRIGGER& pData){return false;};
	/**
		*Cancel previous event trigger request \n
		*Not implement yet
		*
	*/
	virtual bool __stdcall UnReqTrigger(){return false;};
};


class CIDMIPCClientEx : public CIDMIPCClient
{
public:
	virtual bool __stdcall RegClientCallBackExt(CIDMIPCClientCallBackExt* pCallBack){return false;};

	virtual bool __stdcall GrabFrameEx(UINT uCam,UINT uSizeID,UINT uFormat/*DMIPC_CAM_FORMAT_RGB24 DMIPC_CAM_FORMAT_YUV */
										,bool bSync=false,IPC_RPY_CAM** pRpyCam=NULL,LPVOID* pData=NULL){return false;};
	virtual bool __stdcall UpdateConfig(UINT uConfig,const LPDMData& pDMData,DWORD dwReserved,bool bSync=false,BOOL* pbSucceed=NULL,LPDMData* pData=NULL){return false;};
};
#endif // _IDMIPCCLIENT_H_
