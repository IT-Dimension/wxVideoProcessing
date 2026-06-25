#pragma once
#include <wxGuiPluginBase.h>

class MotionDetectorMultiCamGuiPlugin : public wxGuiPluginBase
{
	DECLARE_DYNAMIC_CLASS(MotionDetectorMultiCamGuiPlugin)
public:
	MotionDetectorMultiCamGuiPlugin(wxEvtHandler* handler = nullptr);
	virtual ~MotionDetectorMultiCamGuiPlugin();

	virtual wxString GetName() const override;
	virtual wxString GetId() const override;
	virtual wxWindow* CreatePanel(wxWindow* parent) override;
};