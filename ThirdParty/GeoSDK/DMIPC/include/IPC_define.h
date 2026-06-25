#ifndef IPC_DEFINE_H
#define IPC_DEFINE_H

#define DMIPC_VER	5000		///< DLL version
#define DMIPC_MAX_CHANNEL	16

#define DMIPC_MAX_IO_DEVICE	9
#define DMIPC_MAX_INPUT		16
#define DMIPC_MAX_OUTPUT	16
/** @defgroup Define Defined values
 *  All definations
 *  @{
 */
/** @defgroup DefineMessage Messages
 *  @ingroup Define
 *  Messages used by DMDataHeader.message
   *  @{
   */

#define	DMIPC_MSG_NONE			0	///< undefined message
#define	DMIPC_MSG_REQUEST_CMD	1	///< request command
#define	DMIPC_MSG_REQUEST_CAM	2	///< request camera
#define	DMIPC_MSG_REQUEST_AUDIO	3	///< request audio
#define	DMIPC_MSG_REQUEST_TRIGGER	10	///< request trigger

#define	DMIPC_MSG_REPLY_OFFSET	255

#define	DMIPC_MSG_REPLY_FAIL	DMIPC_MSG_NONE + DMIPC_MSG_REPLY_OFFSET				///< reply fail
#define	DMIPC_MSG_REPLY_CMD		DMIPC_MSG_REQUEST_CMD + DMIPC_MSG_REPLY_OFFSET		///< reply command
#define	DMIPC_MSG_REPLY_CAM		DMIPC_MSG_REQUEST_CAM + DMIPC_MSG_REPLY_OFFSET		///< reply camera
#define	DMIPC_MSG_REPLY_AUDIO	DMIPC_MSG_REQUEST_AUDIO + DMIPC_MSG_REPLY_OFFSET	///< reply audio
#define	DMIPC_MSG_REPLY_TRIGGER	DMIPC_MSG_REQUEST_TRIGGER + DMIPC_MSG_REPLY_OFFSET	///< reply trigger
//reply = request+255
/** @} */ // end of DefineMessage


/** @defgroup DefinePara Parameters
 *  @ingroup Define
 *  Parameters used by DMDataHeader.para
   *  @{
   */
#define DMIPC_PARA_NONE			0
/** @defgroup DefineParaCMD Parameters for commands
 *  @ingroup DefinePara
 *  Parameters used for DMIPC_MSG_REQUEST_CMD 
   *  @{
   */
#define	DMIPC_PARA_CONTROL		1	///<ask for control
#define	DMIPC_PARA_LIST			2	///<camera lists
#define	DMIPC_PARA_PTZ			3	///<do ptz command
//#define	DMIPC_PARA_EVENT		4	///<event list history (expired)
#define DMIPC_PARA_IO_LIST		4	///<io module lists	(not implement)
#define	DMIPC_PARA_IO			5	///<do output command
#define	DMIPC_PARA_IO_STAT		6	///<get current io status
#define	DMIPC_PARA_CAM_STAT		7	///<get current camera status

#define DMIPC_PARA_CONFIG		8	///<set configurations	(not implement)
#define DMIPC_PARA_SERVICE		9	///<network service start/stop(not implement)
#define DMIPC_PARA_MONITOR		10	///<camera monitor start/stop(not implement)

#define DMIPC_PARA_LOG			11	///<net service notify multicam to log the client information
#define DMIPC_PARA_RCL_DATE		12	///<get lastest recycle date and the oldest file date in DB
#define DMIPC_PARA_SCHEDULE		13	///<schedule start/stop(not implement)
#define DMIPC_PARA_IO_MONITOR	14	///<start/stop io monitor(not implement)

#define DMIPC_PARA_PTZ_LIST		15	///<ptz dome list

#define DMIPC_PARA_IMG_SIZE		16	///<query for suitable image size
#define DMIPC_PARA_IO_DISABLE	17	///<Disable IO module

#define DMIPC_PARA_ASK_TRIGGER	18		//ask for trigger response

#define DMIPC_PARA_CLOSE_FILE	19	///<ask for close current recording file

#define DMIPC_PARA_PTZ_LIST_EXT		20	///<ptz dome list


#define DMIPC_PARA_LIST_EXT		21		///camera lists with IPCam/Jigsaw
#define DMIPC_PARA_RECORD		22		///<start/stop recording (only work when DVR under Monitor/Motion-Detect mode)

#define DMIPC_CMD_PARA_COUNT	23
/** @} */ // end of DefineParaCMD

/** @defgroup DefineParaCAM Parameters for cameras and audios
 *  @ingroup DefinePara
 *  Parameters used for DMIPC_MSG_REQUEST_CAM and DMIPC_MSG_REQUEST_AUDIO
   *  @{
   */
#define	DMIPC_PARA_CAM1			1	
#define	DMIPC_PARA_CAM2			2
#define	DMIPC_PARA_CAM3			3
#define	DMIPC_PARA_CAM4			4
#define	DMIPC_PARA_CAM5			5
#define	DMIPC_PARA_CAM6			6
#define	DMIPC_PARA_CAM7			7
#define	DMIPC_PARA_CAM8			8
#define	DMIPC_PARA_CAM9			9
#define	DMIPC_PARA_CAM10		10
#define	DMIPC_PARA_CAM11		11
#define	DMIPC_PARA_CAM12		12
#define	DMIPC_PARA_CAM13		13
#define	DMIPC_PARA_CAM14		14
#define	DMIPC_PARA_CAM15		15
#define	DMIPC_PARA_CAM16		16
#define DMIPC_CAM_PARA_COUNT	17

#define DMIPC_PARA_JIGSAW1		129
#define DMIPC_PARA_JIGSAW2		130
#define DMIPC_PARA_JIGSAW3		131
#define DMIPC_PARA_JIGSAW4		132
#define DMIPC_CAM_JIGSAW_PARA_COUNT	4
/** @} */ // end of DefineParaCAM

/** @defgroup DefineParaTrigger Parameters for triggers
 *  @ingroup DefinePara
 *  Parameters used for DMIPC_MSG_REPLY_TRIGGER
   *  @{
   */
#define DMIPC_PARA_IMAGELOST	1			//video lost trigger
#define	DMIPC_PARA_MOTION		2			//motion detected trigger
#define	DMIPC_PARA_IOLOST		3			//IO lost trigger
#define	DMIPC_PARA_HDDFULL		4			//HDD full trigger
#define	DMIPC_PARA_IO			5			//IO states changed trigger

#define DMIPC_PARA_ON_CAMMONITOR	6	//recording start/stop trigger
#define DMIPC_PARA_ON_RECYCLE		7	//starting recycle
#define DMIPC_PARA_ON_IOMONITOR	8	//io monitor start/stop trigger
#define DMIPC_PARA_ON_SCHEDULEMONITOR	9	//schedule monitor start/stop trigger
#define DMIPC_PARA_ON_SOURCECHANGE	10		//video source changed

#define DMIPC_PARA_ON_OBJ_EVENT		11		//event about object
#define DMIPC_PARA_ON_POS_EVENT		12		//event about pos

#define DMIPC_PARA_ON_SCENE_CHANGE	13
#define DMIPC_PARA_ON_CONFIG_CHANGE	14

#define DMIPC_PARA_ON_IO_DISABLE	15		//IO pin disabled trigger
#define DMIPC_PARA_ON_LOGIN			16		//multicam login user changed/fail

#define DMIPC_PARA_ON_RESTART		17		//multicam auto restarting
#define DMIPC_PARA_ON_OUTPUT_RESET		18		//OUTPUT reset

#define DMIPC_PARA_ON_POS_DATA		19		//event about pos message

#define DMIPC_PARA_ON_IMG_PROC			20

#define DMIPC_PARA_ON_USB_CHANGE		21

#define DMIPC_TRIGGER_PARA_COUNT	22

#define DMIPC_FATAL_ERROR		200	//fatal error trigger

/*
FATAL ERROR IDs
*/
#define DMIPC_FATAL_ERROR_ID_NONE		0
#define DMIPC_FATAL_ERROR_ID_CODEC		1
#define DMIPC_FATAL_ERROR_ID_CREATEFILE	2
#define DMIPC_FATAL_ERROR_ID_WRITEFILE	3

#define DMIPC_FATAL_ERROR_ID_UNKNOWN	255

/** @} */ // end of DefineParaTrigger


//for Camera size and ID
#define DMIPC_CAM_FORMAT_RGB24	1
#define DMIPC_CAM_FORMAT_YUV	2

#define DMIPC_CAM_SIZE32x24		230400	//image size
#define DMIPC_CAM_SIZE64x24		460800
#define DMIPC_CAM_SIZE64x48		921600

#define DMIPC_CAM_SIZE36x24		259200	//v7.0 NTSC
#define DMIPC_CAM_SIZE72x24		518400	
#define DMIPC_CAM_SIZE72x48		1036800

#define DMIPC_CAM_SIZE360x288	311040	//v7.0 PAL
#define DMIPC_CAM_SIZE720x288	622080	
#define DMIPC_CAM_SIZE720x576	1244160

#define DMIPC_CAM_SIZE384x288	331776	//reserved
#define DMIPC_CAM_SIZE768x576	1327104

#define DMIPC_CAM_SIZE_MEGA_PIXEL	(UINT)(-1)


//IPC_RPY_CAM::uOriginalSizeID
#define DMIPC_CAM_SIZEID_S64x48		0	//image size id 64x48 series
#define DMIPC_CAM_SIZEID_S72x48		10	//image size id 72x48 series
#define DMIPC_CAM_SIZEID_S72x57		20	//image size id 72x57 series

#define DMIPC_CAM_SIZEID32x24		0	//image size id
#define DMIPC_CAM_SIZEID64x24		1
#define DMIPC_CAM_SIZEID64x48		2
#define DMIPC_CAM_SIZEID80x60		3	//800x600
#define DMIPC_CAM_SIZEID1024x768	4	//1024x768
#define DMIPC_CAM_SIZEID1280x960	5	//1280x960

#define DMIPC_CAM_SIZEID36x24		10
#define DMIPC_CAM_SIZEID72x24		11
#define DMIPC_CAM_SIZEID72x48		12
#define DMIPC_CAM_SIZEID1152x768	14	//1152x768

#define DMIPC_CAM_SIZEID360x288		20
#define DMIPC_CAM_SIZEID720x288		21
#define DMIPC_CAM_SIZEID720x576		22

#define DMIPC_CAM_SIZEID_MEGA_PIXEL	200 //ask for mega pixel size
#define DMIPC_CAM_SIZEID_CROPPING	201 //for mega-pixel size(reserved)
#define DMIPC_CAM_SIZEID_ENCODED	202	//request for encoded data(MJPG) fill in IPC_REQ_CAM::uSizeID
#define DMIPC_CAM_SIZEID_ENCODED_MPG		203	//request for encoded data (MPEG4)	
#define DMIPC_CAM_SIZEID_ENCODED_RELEASE	204	//fill in IPC_REQ_CAM::uSizeID	for release server resource

//#define DMIPC_CAM_SIZEID_SCALE		 //for mega-pixel size()

#define DMIPC_CAM_SIZEID384x288		255	//reserved
#define DMIPC_CAM_SIZEID768x576		255	//reserved

#define DMIPC_CAM_SIZE_COUNT	6

//for ptz command
#define G_DoCommand_None		0
#define G_DoCommand_PanLeft		1
#define G_DoCommand_PanRight	2
#define G_DoCommand_TiltUp		3
#define G_DoCommand_TiltDown	4
#define G_DoCommand_ZoomTele	5
#define G_DoCommand_ZoomWide	6
#define G_DoCommand_Home		7
#define G_DoCommand_Auto		8	
#define G_DoCommand_FocusNear	9	
#define G_DoCommand_FocusFar	10
#define G_DoCommand_AUX			11		
#define G_DoCommand_Zoom		12			
#define G_DoCommand_FocusStop		13		//do not use this
#define G_DoCommand_PresetGo		14		
#define G_DoCommand_PresetSet		15		//do not use this
#define G_DoCommand_FocusAuto		16

#define G_DoCommand_Auto_Stop		17
#define G_DoCommand_Auto_Def1		18
#define G_DoCommand_Auto_Def2		19
#define G_DoCommand_Auto_Def3		20
#define G_DoCommand_Auto_Def4		21
#define G_DoCommand_Auto_Def5		22
#define G_DoCommand_Auto_Def6		23
#define G_DoCommand_Auto_Def7		24
#define G_DoCommand_Auto_Def8		25
#define G_DoCommand_Auto_Def9		26

#define	G_DoCommand_RLY1			24	//aux on
#define G_DoCommand_AUX_ON			24
#define	G_DoCommand_RLY2			25	//aux off
#define G_DoCommand_AUX_OFF			25
#define	G_DoCommand_RLY3			26
#define	G_DoCommand_12V1			27
#define	G_DoCommand_12V2			28
#define	G_DoCommand_12V3			29
#define	G_DoCommand_12V4			30

#define	G_DoCommand_DO1				31
#define	G_DoCommand_DO2				32
#define	G_DoCommand_DO3				33
#define	G_DoCommand_DO4				34
#define	G_DoCommand_DO5				35
#define	G_DoCommand_DO6				36
#define	G_DoCommand_DO7				37
#define	G_DoCommand_DO8				38
#define	G_DoCommand_DO9				39
#define	G_DoCommand_DO10			40
#define	G_DoCommand_DO11			41
#define	G_DoCommand_DO12			42
#define	G_DoCommand_DO13			43
#define	G_DoCommand_DO14			44
#define	G_DoCommand_DO15			45
#define	G_DoCommand_DO16			46

#define	G_DoCommand_PresetGo1			47
#define	G_DoCommand_PresetGo2			48
#define	G_DoCommand_PresetGo3			49
#define	G_DoCommand_PresetGo4			50
#define	G_DoCommand_PresetGo5			51
#define	G_DoCommand_PresetGo6			52
#define	G_DoCommand_PresetGo7			53
#define	G_DoCommand_PresetGo8			54
#define	G_DoCommand_PresetGo9			55
#define	G_DoCommand_PresetGo10			56
#define	G_DoCommand_PresetGo11			57
#define	G_DoCommand_PresetGo12			58
#define	G_DoCommand_PresetGo13			59
#define	G_DoCommand_PresetGo14			60
#define	G_DoCommand_PresetGo15			61
#define	G_DoCommand_PresetGo16			62

#define	G_DoCommand_Auto1				63
#define	G_DoCommand_Auto2				64
#define	G_DoCommand_Auto3				65
#define	G_DoCommand_Auto4				66
#define	G_DoCommand_Auto5				67
#define	G_DoCommand_Auto6				68
#define	G_DoCommand_Auto7				69
#define	G_DoCommand_Auto8				70
#define	G_DoCommand_Auto9				71
#define	G_DoCommand_Auto10				72
#define	G_DoCommand_Auto11				73
#define	G_DoCommand_Auto12				74
#define	G_DoCommand_Auto13				75
#define	G_DoCommand_Auto14				76
#define	G_DoCommand_Auto15				77
#define	G_DoCommand_Auto16				78

#define G_DoCommand_LeftUp		79
#define G_DoCommand_RightUp		80
#define G_DoCommand_LeftDown	81
#define G_DoCommand_RightDown	82

#define G_DoCommand_STOP_OFFSET		100


#define DMIPC_OUTPUT_CMD_MODE_PULSE		0		// send on/off message
#define DMIPC_OUTPUT_CMD_MODE_ON		1		// send on
#define DMIPC_OUTPUT_CMD_MODE_RESET		2		// send reset
#define DMIPC_OUTPUT_CMD_MODE_OFF		3		// send off

#define DMIPC_SERVER_HEALTH_CANCEL		-3
#define DMIPC_SERVER_HEALTH_BUSY		-2
#define DMIPC_SERVER_HEALTH_TIMEDOUT	-1
#define DMIPC_SERVER_HEALTH_OK				0
#define DMIPC_SERVER_HEALTH_ERROR			1
#define DMIPC_SERVER_HEALTH_UI_DEAD			2
#define DMIPC_SERVER_HEALTH_RECORD_DEAD		3
#define DMIPC_SERVER_HEALTH_ABANDONED		4
#define DMIPC_SERVER_HEALTH_VIDEO_LOST		5
#define DMIPC_SERVER_HEALTH_VIDEO_IN_DEAD	6
#define DMIPC_SERVER_HEALTH_COD_DEAD		7

/** @} */ // end of Define
#endif