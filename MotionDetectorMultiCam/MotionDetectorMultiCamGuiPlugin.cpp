#include "stdwx.h"
#include "MotionDetectorMultiCamGuiPlugin.h"
#include "MultiCamSettingsPanel.h"

IMPLEMENT_DYNAMIC_CLASS(MotionDetectorMultiCamGuiPlugin, wxGuiPluginBase)

MotionDetectorMultiCamGuiPlugin::MotionDetectorMultiCamGuiPlugin(wxEvtHandler* handler)
	: wxGuiPluginBase(handler)
{
}

MotionDetectorMultiCamGuiPlugin::~MotionDetectorMultiCamGuiPlugin()
{
}

wxString MotionDetectorMultiCamGuiPlugin::GetId() const
{
	return wxT("{DC7382A2-1813-414c-9BD4-4BDF09058A0E}");
}

wxString MotionDetectorMultiCamGuiPlugin::GetName() const
{
	return wxT("MultiCam Motion Detector");
}

wxWindow* MotionDetectorMultiCamGuiPlugin::CreatePanel(wxWindow* parent)
{
	MultiCamSettingsPanel* panel = new MultiCamSettingsPanel(nullptr, parent);

	return static_cast<wxWindow*>(panel);
}

