#include "stdwx.h"
#include "MotionDetectorMultiCamPlugin.h"
#include "MotionDetectorMultiCam.h"
#include "MultiCamSettingsPanel.h"
#if defined(USE_VLD)
#include <vld.h>s
#endif

IMPLEMENT_DYNAMIC_CLASS(MotionDetectorMultiCamPlugin, MotionDetectorPluginBase)

MotionDetectorMultiCamPlugin::MotionDetectorMultiCamPlugin()
: m_ID(wxT("{DC7382A2-1813-414c-9BD4-4BDF09058A0E}"))
{

}

MotionDetectorMultiCamPlugin::~MotionDetectorMultiCamPlugin()
{

}

wxString MotionDetectorMultiCamPlugin::GetID() const
{
	return m_ID;
}

wxString MotionDetectorMultiCamPlugin::GetName() const
{
	return _("MultiCam Motion Detector");
}

MotionDetectorBase * MotionDetectorMultiCamPlugin::CreateDetector(GeometryProvider && geometryProvider)
{
	return new MotionDetectorMultiCam(this, std::move(geometryProvider));
}

