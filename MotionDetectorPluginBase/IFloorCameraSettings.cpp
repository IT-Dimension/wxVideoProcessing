#include "stdwx.h"
#include "IFloorCameraSettings.h"

IMPLEMENT_DYNAMIC_CLASS(IFloorCameraSettings, xsSerializable)

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(IFloorCameraSettingsList);

IFloorCameraSettings::IFloorCameraSettings(void)
: bEnabled(false)
, TopLeftX(0),		TopLeftY(0),	TopRightX(639),		TopRightY(0)
, BottomLeftX(0),	BottomLeftY(479),	BottomRightX(639),	BottomRightY(479)
, WinWidth(640), WinHeight(480)
, CamWidth(640), CamHeight(480)
, bVerticalMirror(false)
, bHorizontalMirror(false)
{
	InitSerialization();
}

IFloorCameraSettings::IFloorCameraSettings(const IFloorCameraSettings & settings)
{
	InitSerialization();
	CopyFrom(settings);
}

void IFloorCameraSettings::InitSerialization()
{
	XS_SERIALIZE(bEnabled, wxT("bEnabled"));

	XS_SERIALIZE(TopLeftX, wxT("TopLeftX"));
	XS_SERIALIZE(TopLeftY, wxT("TopLeftY"));
	XS_SERIALIZE(TopRightX, wxT("TopRightX"));
	XS_SERIALIZE(TopRightY, wxT("TopRightY"));
	XS_SERIALIZE(BottomLeftX, wxT("BottomLeftX"));
	XS_SERIALIZE(BottomLeftY, wxT("BottomLeftY"));
	XS_SERIALIZE(BottomRightX, wxT("BottomRightX"));
	XS_SERIALIZE(BottomRightY, wxT("BottomRightY"));
	XS_SERIALIZE(WinWidth, wxT("WinWidth"));
	XS_SERIALIZE(WinHeight, wxT("WinHeight"));
	XS_SERIALIZE(CamWidth, wxT("CamWidth"));
	XS_SERIALIZE(CamHeight, wxT("CamHeight"));

	XS_SERIALIZE(bVerticalMirror, wxT("bVerticalMirror"));
	XS_SERIALIZE(bHorizontalMirror, wxT("bHorizontalMirror"));

	XS_SERIALIZE(DeviceID, wxT("DeviceID"));
	XS_SERIALIZE(VideoSourceID, wxT("VideoSourceID"));
}

IFloorCameraSettings& IFloorCameraSettings::operator=(const IFloorCameraSettings & settings)
{
	if (&settings != this)
		CopyFrom(settings);
	return *this;
}

IFloorCameraSettings::~IFloorCameraSettings(void)
{
}

bool IFloorCameraSettings::Deserialize(wxInputStream & instream)
{
	return SerializableBase::Deserialize(instream, *this);
}

bool IFloorCameraSettings::Serialize(wxOutputStream & outstream)
{
	return SerializableBase::Serialize(outstream, *this);
}

void IFloorCameraSettings::CopyFrom(const IFloorCameraSettings& settings)
{
	bEnabled = settings.bEnabled;
	TopLeftX = settings.TopLeftX;
	TopLeftY = settings.TopLeftY;
	TopRightX = settings.TopRightX;
	TopRightY = settings.TopRightY;
	BottomLeftX = settings.BottomLeftX;
	BottomLeftY = settings.BottomLeftY;
	BottomRightX = settings.BottomRightX;
	BottomRightY = settings.BottomRightY;
	WinWidth = settings.WinWidth;
	WinHeight = settings.WinHeight;
	CamWidth = settings.CamWidth;
	CamHeight = settings.CamHeight;
	bVerticalMirror = settings.bVerticalMirror;
	bHorizontalMirror = settings.bHorizontalMirror;
	VideoSourceID = settings.VideoSourceID;
	DeviceID  =  settings.DeviceID;
}