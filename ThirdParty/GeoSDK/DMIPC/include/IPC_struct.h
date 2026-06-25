#ifndef IPC_STRUCT_H
#define IPC_STRUCT_H
//define common struct for IPC

#pragma pack(push,1)
typedef struct tagWAVEFORMATEX
{
    WORD    wFormatTag;        /* format type */
    WORD    nChannels;         /* number of channels (i.e. mono, stereo...) */
    DWORD   nSamplesPerSec;    /* sample rate */
    DWORD   nAvgBytesPerSec;   /* for buffer estimation */
    WORD    nBlockAlign;       /* block size of data */
    WORD    wBitsPerSample;    /* Number of bits per sample of mono data */
    WORD    cbSize;            /* The count in bytes of the size of
                                    extra information (after cbSize) */

} IPC_WAVEFORMATEX;
#pragma pack(pop)
/** @defgroup Struct Common structures
 *  Common structures used by DMIPC
 *  @{
 */

/** @defgroup InitStruct The Initialize struct
 *  @ingroup Struct
 *	Struct of Initialize data use by CreateIDMIPC
  *  @{
 */
/**
* Initialize structure
*/
#pragma pack(push,8)
typedef struct IDMIPCInitData{
	UINT uID;				///<[in/out]owner app id ,reserved any value will be ok
	/**
		*[in] client request camera numbers \n
		*[out] server replied for valid camera numbers \n
		*if the client filled this parameter larger than the server provided \n
		*it will be modified to the number of server provided \n
		*if the client filled it smaller than the server provided,no modification will be occerrd\n
		*and the client can only request the number of camera even though the server provided more 
	*/
	UINT uCamCount;			///<[in/out]camera count 
	/**
		*see uCamCount
	*/
	UINT uAudioCount;		///<[in/out]audio count
	/**
		*allowed size \n
		*0 For Server Default
		*DMIPC_CAM_SIZE64x48 (640x480x3) \n
		*DMIPC_CAM_SIZE64x24 (640x240x3) \n
		*DMIPC_CAM_SIZE32x24 (320x240x3) \n
		*DMIPC_CAM_SIZE36x24			//v7.0 NTSC	\n 
		*DMIPC_CAM_SIZE72x24			
		*DMIPC_CAM_SIZE72x48		
		*DMIPC_CAM_SIZE360x288			//v7.0 PAL	\n
		*DMIPC_CAM_SIZE720x288	
		*DMIPC_CAM_SIZE720x576	

		*modified if client fill an invalid value or server do not support the size
		*
	*/
	UINT uImageBufSize;		///<[in/out]bitmap size
	/**
		*[in] don't care \n
		*[out] audio buffer size \n
		*leave this for default value will be better
	*/						
	UINT uAudioBufSize;		///<[in/out]audio buffer size
	/**
		*[in] don't care \n
		*[out] command buffer size \n
		*leave this for default value will be better
	*/						
	UINT uCmdBufSize;		///<[in/out]cmd buf size
	/**
		*[in] don't care \n
		*[out] trigger buffer size \n
		*leave this for default value will be better
	*/						
	UINT uTriggerBufSize;	///<[in/out]trigger buf
	char sID[33];			///<[in]reserved
	char sPwd[33];			///<[in]reserved
	UINT uMaxServiceNumber;	///<[out]max service count	
	UINT uReserved1;		///<reserved must be 0
	UINT uReserved2;		///<reserved must be 0
	UINT uReserved3;		///<reserved must be 0
	UINT uReserved4;		///<reserved must be 0
	IDMIPCInitData():
		uID(0),uCamCount(DMIPC_MAX_CHANNEL),uAudioCount(0)
			,uImageBufSize(DMIPC_CAM_SIZE64x48/*640*480*3*/),
			uAudioBufSize(8192),uCmdBufSize(4096),uTriggerBufSize(2048),
			uMaxServiceNumber(1)
		{
			uReserved1=0;
			uReserved2=0;
			uReserved3=0;
			uReserved4=0;
		}
}IDMIPCInitData,*LPIDMIPCInitData;
/** @} */ // end of InitStruct
/** @} */ // end of Struct

/** @defgroup RequestHeaderStruct Request header
 *  @ingroup Struct
 *  Request header
   *  @{
   */

  /*!
   *  Request header base struct
   */

typedef struct DMRequest{
	UINT requster;	///< reserved skip it
	UINT message;	///< message to multicam 
	UINT para;		///< parameter if nessecary
	DMRequest():requster(0),message(0),para(0){}
}DMRequest,* LPDMRequest;

/*!
   *  Request header struct  \n
   *  Use this struct as the parameter of  CIDMIPCClient::Request
   */
typedef struct DMDataHeader : public DMRequest{
	UINT uReserved;	///<reserved skip it
}DMDataHeader,*LPDMDataHeader;	
/** @} */ // end of Struct


/** @defgroup RequestBodyStruct Request Data
	*@ingroup Struct 
	*Request data
   *  @{
   */

  /*!
   *  Request data base struct
   */
typedef struct DMData {
	UINT uSize;		///<must fill to the real size of derived struct 
	DMData():uSize(0){}
}DMData,*LPDMData;

typedef struct IPC_REQ_LIST: DMData{
	DWORD	reserved;
}IPC_REQ_LIST;

typedef struct IPC_RPY_LIST: DMData{
	WORD	nTrack;
	WORD	nTrackID[32];
	char	strTrackName[32][64];
	WORD	SizeX;
	WORD	SizeY;
}IPC_RPY_LIST;

#if (DMIPC_VER >= 5000)
typedef struct IPC_REQ_LIST_EXT: DMData{
	DWORD	reserved;

}IPC_REQ_LIST_EXT;

#define IPC_LIST_IMAGE_DEFOG		1
#define IPC_LIST_IMAGE_STABILITY	2

#define IPC_LIST_CAM_TYPE_DVR				1
#define IPC_LIST_CAM_TYPE_GEOIPCAM			2
#define IPC_LIST_CAM_TYPE_IPCAM				3
#define IPC_LIST_CAM_TYPE_JIGSAW			4

#define IPC_LIST_AUDIO_CODEC_NOAUDIO	0
#define IPC_LIST_AUDIO_CODEC_PCM	1

#define IPC_LIST_VIDEO_ARRTIB_SET_BRIGHTNESS	1
#define IPC_LIST_VIDEO_ARRTIB_SET_CONTRAST		2
#define IPC_LIST_VIDEO_ARRTIB_SET_STAURATION	4
#define IPC_LIST_VIDEO_ARRTIB_SET_HUE			8
#define IPC_LIST_VIDEO_ARRTIB_SET_SHARPNESS		16

typedef struct IPC_RPY_LIST_EXT: DMData	
{

	WORD	nTrack;				//total valid cam count(max 16 now)
	DWORD	dwIPCamMask;		//bit mask for ipcam
	DWORD	dwIPCamCount;		//IPCam count
	DWORD	dwJigsawCount;		//Jigsaw count
	DWORD	dwJigsawMask;		//bit mask for jigsaw
	DWORD	dwReserved1;
	DWORD	dwReserved2;
	DWORD	dwReserved3;
	struct {
		WORD	nTrackID;		//camera index
		WORD	nCameraType;	//camera type
		DWORD	dwImageOption;	//defog,stability etc		
		DWORD	dwVideoAttribSetting;	//reserved
		WORD	SizeX;			//max image size X(raw data)
		WORD	SizeY;			//max image size Y(raw data)
		DWORD	dwCodec;		//motion jpeg
		DWORD	dwStreamCodec;	//video codec 4CC
		WORD	SizeXCoded;		//coded size X
		WORD	SizeYCoded;		//coded size Y
		DWORD	dwAudioCodec;	//audio codec bitMask
		char	strName[64];
	} sExtraCamInfo[1];			//DVR(ipcam) + jigsaw

}IPC_RPY_LIST_EXT;

#endif


//DMIPC_PARA_IO_LIST
typedef struct IPC_REQ_IO_LIST: DMData{
	DWORD	reserved;
}IPC_REQ_IO_LIST;

typedef struct IPC_RPY_IO_LIST: DMData{
	WORD	nModules;
	typedef struct IPC_PINNAME{
		int iInputPinCnt;
		int iOutputPinCnt;
		char strInputPinName[DMIPC_MAX_INPUT][17];
		char strOutputPinName[DMIPC_MAX_OUTPUT][17];
		IPC_PINNAME():iInputPinCnt(0),iOutputPinCnt(0){}
	}IPC_PINNAME;
	IPC_PINNAME Module[DMIPC_MAX_IO_DEVICE];	
}IPC_RPY_IO_LIST;

typedef struct IPC_REQ_CAM: DMData{
	UINT	uSizeID;		//0:320x240	1:640x240	2:640x480	255:check reserved	
	DWORD	reserved;		//(yuy2)0:320x240	1:640x240	2:640x480

							//if uSizeID==DMIPC_CAM_SIZEID_ENCODED_MPG or DMIPC_CAM_SIZEID_ENCODED_RELEASE
							//down-cast to IPC_REQ_CAM_EXT
}IPC_REQ_CAM;

typedef struct IPC_RPY_CAM: DMData	
{
	SYSTEMTIME	time;
	/*
	if time.wYear==0		==> camera not actived
	if time.wYear==(WORD)-1	==> video lost
	if time.wYear==(WORD)-2	==> video not updated
	if time.wYear==(WORD)-3	==> mepg stream handle released ok
	in these cases "bmpHeader" and the "LPVOID" bmp raw data are not available
	*/
	BITMAPINFOHEADER bmpHeader;
	UINT	uOriginalSizeID;	//0:320x240	1:640x240	2:640x480	
								//if uOriginalSizeID==DMIPC_CAM_SIZEID_ENCODED_MPG : down-cast to IPC_RPY_CAM_EXT
								
}IPC_RPY_CAM;

#if (DMIPC_VER >= 5000)
typedef struct IPC_REQ_CAM_EXT: IPC_REQ_CAM{

	union {
		struct IPC_REQ_CAM_EXT_MPEG{
			DWORD	dwHandleHigh;
			DWORD	dwHandleLow;
			DWORD	dwReserved;
			DWORD	dwReserved1;
		}mpegData;
	
		struct IPC_REQ_CAM_EXT_CROP{	//reserved
			DWORD	dwX;			//left up X point
			DWORD	dwY;			//left up Y point
			DWORD	dwWidth;		//width	0 for original width
			DWORD	dwHeight;		//height 0 for original height
		}cropData;
		
	}Args;	

}IPC_REQ_CAM_EXT;

typedef struct IPC_RPY_CAM_EXT: IPC_RPY_CAM	
{
	DWORD	dwValidCamMask;		//reserved 0
	DWORD	dwBuffType;			//reserved 0
	DWORD	dwBufSize;			//size of pData 

	union {
		struct IPC_REQ_CAM_EXT_MPEG{
			BOOL	bKey;			//is key frame
			DWORD	dwSerialNum;	//serial number
			DWORD	dwHandleHigh;	
			DWORD	dwHandleLow;
		}mpegData;

		struct IPC_REQ_CAM_EXT_CROP{	//reserved
			DWORD	dwX;			//left up X point
			DWORD	dwY;			//left up Y point
			DWORD	dwWidth;		//width	0 for original width
			DWORD	dwHeight;		//height 0 for original height
		}cropData;
	}Args;	
	
}IPC_RPY_CAM_EXT;

#endif


typedef struct IPC_REQ_AUDIO: DMData{
	DWORD	reserved;	
}IPC_REQ_AUDIO;

typedef struct IPC_RPY_AUDIO: DMData
{
	IPC_WAVEFORMATEX wfx;
	SYSTEMTIME	time;
	DWORD		dwDataSize;
}IPC_RPY_AUDIO;


typedef struct IPC_REQ_PTZ: DMData{
	WORD	nCommand;		//ptz command
	WORD	nDevice;		//0
	WORD	nAddr;			//0
	WORD	nCam;			//1-16
#if (DMIPC_VER > 1220)
#define IPC_PTZ_SPEED_OFFSET	10
#define IPC_PTZ_FIXED_SPEED_OFFSET	20
	WORD	nSpeed;			//PT speed 1(low)~5(high) , 0 for default	,auto stop mode
							// +IPC_PTZ_SPEED_OFFSET for non-stop mode, 10 for default , 11(low)~15(high)
							// +IPC_PTZ_FIXED_SPEED_OFFSET for non speed up mode, 20 for default , 21(low)~25(high)
#endif

	union {
		DWORD	dwGoIndex;	//G_DoCommand_PresetGo 

//		BOOL bAUX;			//G_DoCommand_AUX
	
		struct IPC_REQ_PTZ_ZOOM{
			WORD	wZoomX;
			WORD	wZoomY;
			WORD	wZoomWidth;
			WORD	wZoomHeight;
		} sPTZZoom;					

	}Args;	
	
}IPC_REQ_PTZ;

//DMIPC_PARA_IO
typedef struct IPC_REQ_IO: DMData{
	WORD	nCommand;		//G_DoCommand_DO1 ~ G_DoCommand_DO16
	WORD	nReserved;		//must be 0
	WORD	nModule;		//module index
	WORD	Reserved2;		//
	WORD	wToggleForceMode;	//0:don't care(pulse on-off) 1:on  2:reset	3:off
#if(DMIPC_VER>=3000)
	BOOL	bByInput;		//是否因input->output
	WORD	nInputModule;		//因input的module	0-9
	WORD	nInputPin;		//因input的pin		0-16
#endif
}IPC_REQ_IO;

typedef struct IPC_PIN{
	BYTE Pin0:1;
	BYTE Pin1:1;
	BYTE Pin2:1;
	BYTE Pin3:1;
	BYTE Pin4:1;
	BYTE Pin5:1;
	BYTE Pin6:1;
	BYTE Pin7:1;
	BYTE Pin8:1;
	BYTE Pin9:1;
	BYTE Pin10:1;
	BYTE Pin11:1;
	BYTE Pin12:1;
	BYTE Pin13:1;
	BYTE Pin14:1;
	BYTE Pin15:1;
}IPC_PIN;

typedef struct IPC_POUT{
	BYTE Out0:1;
	BYTE Out1:1;
	BYTE Out2:1;
	BYTE Out3:1;
	BYTE Out4:1;
	BYTE Out5:1;
	BYTE Out6:1;
	BYTE Out7:1;
	BYTE Out8:1;
	BYTE Out9:1;
	BYTE Out10:1;
	BYTE Out11:1;
	BYTE Out12:1;
	BYTE Out13:1;
	BYTE Out14:1;
	BYTE Out15:1;
}IPC_POUT;

typedef struct IPC_Table{
	bool	bNormal;			//ok(true)/lost(false)
	BYTE	PinCount;			//input pin count
	BYTE	OutCount;			//output pin count
	IPC_PIN Pin;
	IPC_POUT Pout;
}IPC_Table;

//DMIPC_PARA_IO_STAT
typedef struct IPC_RPY_IO_STAT: DMData{
	UINT	uRIOMCount;			//module count
	IPC_Table Table[DMIPC_MAX_IO_DEVICE];
}IPC_RPY_IO_STAT;

//DMIPC_PARA_CAM_STAT
typedef struct IPC_RPY_CAM_STAT: DMData{
	BYTE	CamStatus[DMIPC_MAX_CHANNEL];		//0:noraml 1:not active 2:video lost 3:recording
}IPC_RPY_CAM_STAT;


#define IPC_MONITOR_RPY_OK			0
#define IPC_MONITOR_RPY_HDD_FULL	1
#define IPC_MONITOR_RPY_NO_CONFIG	2

//DMIPC_PARA_MONITOR
typedef struct IPC_REQ_MONITOR: DMData{
	DWORD	dwMaskCamStartMonitor;	    
	DWORD	dwMaskCamStopMonitor;	
	BOOL	bQuery;
#if (DMIPC_VER >= 4100)
	//fill following data if configed by remote 
	DWORD	dwIP;			//client ip
	char	sLoginID[33];	//client id
#endif
}IPC_REQ_MONITOR;

typedef struct IPC_RPY_MONITOR: DMData{
	DWORD	dwMaskCamRecordStatus;
	DWORD	CamAllowConfig;
	DWORD	dwRpyFlag;
}IPC_RPY_MONITOR;
//DMIPC_PARA_IO_MONITOR
typedef struct IPC_REQ_IO_MONITOR : DMData{
	BOOL	bIOMonitor;
	BOOL	bQuery;
#if (DMIPC_VER >= 4100)
	//fill following data if configed by remote 
	DWORD	dwIP;			//client ip
	char	sLoginID[33];	//client id
#endif
}IPC_REQ_IO_MONITOR;

typedef struct IPC_RPY_IO_MONITOR: DMData{
	BOOL	bIOMonitor;
	BOOL	bAllowConfig;
	DWORD	dwRpyFlag;
}IPC_RPY_IO_MONITOR;

//DMIPC_PARA_SCHEDULE
typedef struct IPC_REQ_SCHEDULE : DMData{
	BOOL	bScheduleMonitor;
	BOOL	bQuery;
#if (DMIPC_VER >= 4100)
	//fill following data if configed by remote 
	DWORD	dwIP;			//client ip
	char	sLoginID[33];	//client id
#endif
}IPC_REQ_SCHEDULE;

typedef struct IPC_RPY_SCHEDULE: DMData{
	BOOL	bScheduleMonitor;
	BOOL	bAllowConfig;
	DWORD	dwRpyFlag;		
}IPC_RPY_SCHEDULE;

//DMIPC_PARA_RCL_DATE
typedef struct IPC_RPY_RCL_DATE: DMData{
	SYSTEMTIME	time;			//previous recycle time
	SYSTEMTIME	tKeepDay;		//the oldest file date in All DB Groups now

	BYTE		uGroupCount;		
	struct{
		SYSTEMTIME	time;			//previous recycle time
		DWORD	dwCamMask;			//camera mask in this Group
		SYSTEMTIME	tKeepDay;		//the oldest file date in this Group now
	}	Group[1];
}IPC_RPY_RCL_DATE;

//DMIPC_PARA_PTZ_LIST
typedef struct IPC_REQ_PTZ_LIST : DMData{
	DWORD	dwCamera;			//0 for all list, 1~16 for 
	DWORD	dwReserved;
}IPC_REQ_PTZ_LIST;
//DMIPC_PARA_PTZ_LIST_EXT		
typedef struct IPC_REQ_PTZ_LIST_EXT : DMData{
	DWORD	dwCamera;			//1~16 
	DWORD	dwReserved;
}IPC_REQ_PTZ_LIST_EXT;

typedef struct IPC_RPY_PTZ_LIST : DMData{
	bool	bDetail;			//detail data or not
	DWORD	dwCamera;			//if bDetail is true  : camera index
								//if bDetail is false : mask bit of camera with PTZ dome
	union {
		struct IPC_PTZ_DOME_DATA{
			BOOL	bUD;		//up down 
			BOOL	bLR;		//left-right
			BOOL	bHome;		//home
			BOOL	bZoom;		//zoom
			BOOL	bFocus;		//focus
			BOOL	bAutoFocus;	//auto focus
			BOOL	bSpeed;		//speed
			DWORD	dwPresetCount;
			DWORD	dwAutoTourCount;	
			DWORD	dwOptions;
			DWORD	Reserved;
		} Dome[1];					//if bDetail is false

		struct IPC_PTZ_DOME_DATA_DETAIL{
			char sDeviceName[33];
			char sPresetName[16][33];
			char sAutoName[16][33];
		} Dome_Detail;			//if bDetail is true
	}Data;

}IPC_RPY_PTZ_LIST;

typedef struct IPC_RPY_PTZ_LIST_EXT : DMData{
	DWORD	dwCamera;		
	DWORD	dwPresetCount;
	DWORD	dwAutoTourCount;
	char sDeviceName[33];
	char sPresetDefaultName[33];
	char sAutoDefaultName[33];
	char sName[1][33];		//dwPresetCount+dwAutoTourCount (max=100)
}IPC_RPY_PTZ_LIST_EXT;

//DMIPC_PARA_IMG_SIZE
typedef struct IPC_REQ_IMG_SIZE: DMData{
	DWORD	reserved;		
}IPC_REQ_IMG_SIZE;

typedef struct IPC_RPY_IMG_SIZE: DMData	
{
	DWORD	dwSizeidSeries;	//DMIPC_CAM_SIZEID_S64x48,DMIPC_CAM_SIZEID_S72x48,DMIPC_CAM_SIZEID_S72x57		
	DWORD	dwImgSizeidL;
	DWORD	dwImgSizeidM;
	DWORD	dwImgSizeidS;
	DWORD	reserved;
}IPC_RPY_IMG_SIZE;

//DMIPC_PARA_IO_DISABLE

typedef struct IPC_REQ_IO_DISABLE: DMData{
	DWORD	dwInputEnable[DMIPC_MAX_IO_DEVICE];
	DWORD	dwInputDisable[DMIPC_MAX_IO_DEVICE];
	DWORD	dwOutputEnable[DMIPC_MAX_IO_DEVICE];
	DWORD	dwOutputDisable[DMIPC_MAX_IO_DEVICE];
#if (DMIPC_VER >= 4100)
	//fill following data if configed by remote 
	DWORD	dwIP;			//client ip
	char	sLoginID[33];	//client id
#endif
}IPC_REQ_IO_DISABLE;

typedef struct IPC_RPY_IO_DISABLE: DMData	
{
	WORD	nModules;
	DWORD	dwInputEnable[DMIPC_MAX_IO_DEVICE];
	DWORD	dwOutputEnable[DMIPC_MAX_IO_DEVICE];
}IPC_RPY_IO_DISABLE;


//DMIPC_PARA_ASK_TRIGGER	18		//ask for trigger response
typedef struct IPC_REQ_ASK_TRIGGER: DMData{
	DWORD	dwTriggerType;
}IPC_REQ_ASK_TRIGGER;

typedef struct IPC_RPY_ASK_TRIGGER: DMData	
{
	DWORD	dwTriggerType;
}IPC_RPY_ASK_TRIGGER;


//#define DMIPC_PARA_CLOSE_FILE	19	///<ask for close current recording file
typedef struct IPC_REQ_CLOSE_FILE: DMData{
	DWORD		dwCamera;		//DMIPC_PARA_CAM1 - DMIPC_PARA_CAM16
	SYSTEMTIME	t;				
}IPC_REQ_CLOSE_FILE;

typedef struct IPC_RPY_CLOSE_FILE: DMData	
{
	BOOL		dwFlag;	//TRUE:file closed	,FALSE:not recording(already closed)
	char		sFullPath[_MAX_PATH];		//full path(if dwFlag is TRUE)
}IPC_RPY_CLOSE_FILE;


//DMIPC_PARA_RECORD
typedef struct IPC_REQ_RECORD: DMData{
	DWORD	dwMaskCamStartRecord;	    
	DWORD	dwMaskCamStopRecord;	
#if (DMIPC_VER >= 4100)
	//fill following data if configed by remote 
	DWORD	dwIP;			//client ip
	char	sLoginID[33];	//client id
#endif
}IPC_REQ_RECORD;

typedef struct IPC_RPY_RECORD: DMData{
	DWORD	dwMaskCamRecordStatus;
	DWORD	dwRpyFlag;
/*
dwRpyFlag:
#define IPC_MONITOR_RPY_OK			0
#define IPC_MONITOR_RPY_HDD_FULL	1
*/
}IPC_RPY_RECORD;


//---
//triggers

typedef struct IPC_REQ_TRIGGER: DMData{
	DWORD	dwMaskReqCamMotion;
	DWORD	reserved;
#if (DMIPC_VER > 1220)
	BYTE	sensitivity[DMIPC_MAX_CHANNEL];	//0-10	,0 for use multicam setting
#endif 
}IPC_REQ_TRIGGER;


//#define DMIPC_PARA_IMAGELOST	1
typedef struct IPC_TRIGGER_IMAGELOST: DMData{
	DWORD	dwMaskImageLost;
	SYSTEMTIME	time;	
}IPC_TRIGGER_IMAGELOST;		

//#define	DMIPC_PARA_MOTION		2
typedef struct IPC_TRIGGER_MOTION: DMData{
	DWORD	dwMaskCamMotion;
	SYSTEMTIME	time;
#if (DMIPC_VER > 1220)
	BYTE	sensitivity[DMIPC_MAX_CHANNEL];
#endif 
}IPC_TRIGGER_MOTION;		


//#define	DMIPC_PARA_HDDFULL		4
typedef struct IPC_TRIGGER_HDDFULL: DMData{
	BYTE	uDeviceCount;		//for default buffer size ,this value must less than 100
	SYSTEMTIME	time;
	DWORD	dwCamMask;			//camera mask
	struct{
		TCHAR	szDeviceName[16];
		DWORD	dwSize;			//available size left
	}	Device[1];
}IPC_TRIGGER_HDDFULL;		

//#define	DMIPC_PARA_IO			5	
typedef struct IPC_TRIGGER_IO: DMData{
	IPC_PIN Pin[DMIPC_MAX_IO_DEVICE];
	SYSTEMTIME	time;
#if(DMIPC_VER>=3000)
	IPC_POUT Pout[DMIPC_MAX_IO_DEVICE];
#endif
}IPC_TRIGGER_IO;

//#define	DMIPC_PARA_IOLOST		3
typedef struct IPC_TRIGGER_IOLOST: DMData{
	DWORD	dwMaskIOLost;
	SYSTEMTIME	time;
}IPC_TRIGGER_IOLOST;	
//DMIPC_PARA_ON_CAMMONITOR
typedef struct IPC_TRIGGER_CAM_MONITOR: DMData{
	SYSTEMTIME	time;
	BYTE	Type;		//0:normal ,1: schedule
	DWORD	CamStatus;	//bit mask for which camera under monitoring
	DWORD	CamAllowConfig;	//bit mask for which camera allow config
	DWORD	dwMonitorOption[32];	
		/*bits form low to high means
			record
			alarm
			hotline alert
			output
		*/
}IPC_TRIGGER_CAM_MONITOR;

typedef struct IPC_TRIGGER_IO_MONITOR: DMData{
	SYSTEMTIME	time;
	BYTE	Type;		//0:normal ,1: schedule
	bool 	bStart;		
	bool	bAllowConfig;
	DWORD	dwMonitorOption;	//same as IPC_TRIGGER_CAM_MONITOR::dwMonitorOption
}IPC_TRIGGER_IO_MONITOR;

typedef struct IPC_TRIGGER_RECYCLE: DMData{
	SYSTEMTIME	time;			
	SYSTEMTIME	tKeepDay;		//the oldest file date after recycle
	DWORD		dwStorge;
	DWORD		dwCamMask;
}IPC_TRIGGER_RECYCLE;

typedef struct IPC_TRIGGER_SCHEDULE_MONITOR: DMData{
	SYSTEMTIME	time;		
	BOOL		bStart;			//indicate start/stop
	BOOL		bAllowConfig;
}IPC_TRIGGER_SCHEDULE_MONITOR;

//DMIPC_PARA_ON_SOURCECHANGE
typedef struct IPC_TRIGGER_SOURCE_CHANGE : DMData{
	UINT	uSizeIDNow;		
	/*
#define DMIPC_CAM_SIZEID32x24		0	//image size id
#define DMIPC_CAM_SIZEID64x24		1
#define DMIPC_CAM_SIZEID64x48		2

#define DMIPC_CAM_SIZEID36x24		10
#define DMIPC_CAM_SIZEID72x24		11
#define DMIPC_CAM_SIZEID72x48		12

#define DMIPC_CAM_SIZEID360x288		20
#define DMIPC_CAM_SIZEID720x288		21
#define DMIPC_CAM_SIZEID720x576		22
	*/
}IPC_TRIGGER_SOURCE_CHANGE;

//#define DMIPC_PARA_ON_SCENE_CHANGE	13
typedef struct IPC_TRIGGER_SCENE_CHANGE: DMData{
	SYSTEMTIME	time;	
	DWORD	dwMaskSceneChange;
}IPC_TRIGGER_SCENE_CHANGE;		
//#define DMIPC_PARA_ON_CONFIG_CHANGE	14

#define IPC_TRIGGER_CONFIG_PTZ		1
#define IPC_TRIGGER_CONFIG_IO		2
#define IPC_TRIGGER_CONFIG_GENERAL	4
#define IPC_TRIGGER_CONFIG_CAM_ACTIVE	8
#define IPC_TRIGGER_CONFIG_PASSWORD	16
#define IPC_TRIGGER_CONFIG_POS		32
#define IPC_TRIGGER_CONFIG_IA		64

#define IPC_TRIGGER_CONFIG_IPCAM		128	//ipcam,jigsaw config change


typedef struct IPC_TRIGGER_CONFIG_CHANGE: DMData{
	SYSTEMTIME	time;	
	DWORD	dwMaskConfigChange;
	DWORD	dwReserved;
}IPC_TRIGGER_CONFIG_CHANGE;	

//DMIPC_PARA_ON_IO_DISABLE
typedef struct IPC_TRIGGER_IO_DISABLE: DMData{
	SYSTEMTIME	time;
	WORD	nModules;
	DWORD	dwInputEnable[DMIPC_MAX_IO_DEVICE];
	DWORD	dwOutputEnable[DMIPC_MAX_IO_DEVICE];
}IPC_TRIGGER_IO_DISABLE;	

//DMIPC_PARA_ON_OBJ_EVENT
typedef struct IPC_TRIGGER_OBJECT : DMData{
	SYSTEMTIME	time;
	UINT	uCam;
	UINT	uEventID;
	/*
		Intruder			1
		Object lost			2
		Suspicious object	3
	*/
#if (DMIPC_VER >= 4000)
	DWORD dwFlag;		//填1 (填0為無附加影像資料)
	DWORD dwReserved;	//填0
	DWORD dwSizeX;		//X
	DWORD dwSizeY;		//Y
	char  pData[1];		//附加的影像資料其size=(X*Y*2) yuy2
#endif
}IPC_TRIGGER_OBJECT;

//DMIPC_PARA_ON_RESTART		
typedef struct IPC_TRIGGER_SERVER_RESTART : DMData{
	SYSTEMTIME time;
	UINT	uReserved;		
}IPC_TRIGGER_SERVER_RESTART;

//DMIPC_PARA_ON_OUTPUT_RESET
typedef struct IPC_TRIGGER_OUTPUT_RESET : DMData{
	SYSTEMTIME time;
	WORD		dwResetPinMask[DMIPC_MAX_IO_DEVICE];		
}IPC_TRIGGER_OUTPUT_RESET;

//DMIPC_PARA_ON_LOGIN			
typedef struct IPC_TRIGGER_LOGIN : DMData{
	SYSTEMTIME	time;
	BOOL		bSuccess;
	char		sUserID[33];
}IPC_TRIGGER_LOGIN;

//DMIPC_PARA_ON_POS_EVENT
//DMIPC_PARA_ON_POS_DATA
typedef struct IPC_TRIGGER_POS : DMData{
	SYSTEMTIME	time;
	UINT uEventID;		//reserved
	char sMsgBrief[32];
	char sMsgDetail[256];
#if (DMIPC_VER >= 4000)
	DWORD dwFlag;		//填1 (填0為無附加影像資料)	0:無資料 1:Data為sPosImage 2:Data為sPosExtra
	DWORD dwReserved;	//	dwFlag==2時 為sPosExtra的筆數

	union {
		struct IPC_TRIGGER_POS_EXTRA{
			SYSTEMTIME	time;
			UINT uEventID;		
			char sMsgBrief[32];
			char sMsgDetail[256];
		} sPosExtra[1];					
	
		struct IPC_TRIGGER_POS_IMAGE{
			DWORD dwSizeX;		//X
			DWORD dwSizeY;		//Y
			DWORD dwDataSize;
			char  pData[1];		//附加的影像資料
		} sPosImage;					
	}Data;	
#endif
}IPC_TRIGGER_POS;

//#define DMIPC_PARA_ON_IMG_PROC			20
//#define DMIPC_IMG_PROC_DEFOG		IPC_LIST_IMAGE_DEFOG
//#define DMIPC_IMG_PROC_STABILITY	IPC_LIST_IMAGE_STABILITY

typedef struct IPC_TRIGGER_IMG_PROC: DMData{
	SYSTEMTIME	time;
//	DWORD	dwCam;		//1~16
//	DWORD	dwImgProc;	//bit mask
//	DWORD	dwReserved;
	DWORD	dwMaskDefog;	//bit mask
	DWORD	dwMaskStab;	//bit mask
	DWORD	dwReserved;
	DWORD	dwReserved1;
}IPC_TRIGGER_IMG_PROC;


//DMIPC_PARA_ON_USB_CHANGE		21

typedef struct IPC_TRIGGER_USB_CHANGE: DMData{
	SYSTEMTIME	time;
	BOOL	bInserted;	//TRUE(inserted) / FALSE(removed)
	DWORD	reserved;	
}IPC_TRIGGER_USB_CHANGE;

typedef struct IPC_TRIGGER_FATAL_ERROR : DMData{
	SYSTEMTIME time;
	UINT	uErrorID;		
}IPC_TRIGGER_FATAL_ERROR;

/** @} */ // end of RequestBodyStruct
#pragma pack(pop)
#endif