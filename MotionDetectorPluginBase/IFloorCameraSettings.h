#ifndef _IFLOORCAMERASETTINGS_H
#define _IFLOORCAMERASETTINGS_H

#include "MotionDetectorPlugin.h"
#include <SerializableBase.h>

class IFLOOR_API IFloorCameraSettings : public xsSerializable
{
	DECLARE_DYNAMIC_CLASS(IFloorCameraSettings)
public:
	IFloorCameraSettings(void);
	IFloorCameraSettings(const IFloorCameraSettings & settings);
	IFloorCameraSettings& operator=(const IFloorCameraSettings & settings);

	~IFloorCameraSettings(void);

	bool Deserialize(wxInputStream & instream);
	bool Serialize(wxOutputStream & outstream);

private:
	void InitSerialization();
	void CopyFrom(const IFloorCameraSettings & settings);

public:
	bool bEnabled; // Are these settings enabled

	int
		TopLeftX,
		TopLeftY,
		TopRightX,
		TopRightY,
		BottomLeftX,
		BottomLeftY,
		BottomRightX,
		BottomRightY,
		WinWidth,
		WinHeight,
		CamWidth,
		CamHeight;
	bool
		bVerticalMirror,
		bHorizontalMirror;
	
	wxString
		VideoSourceID,
		DeviceID;
};

WX_DECLARE_USER_EXPORTED_LIST(IFloorCameraSettings, IFloorCameraSettingsList, IFLOOR_API);

#endif // _IFLOORCAMERASETTINGS_H
