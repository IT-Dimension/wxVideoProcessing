#include "stdwx.h"
#include "FilterFactory.h"
#include "FilterTemplate.h"

Filter * FilterFactory::CreateFilter(const wxString & type)
{
	wxClassInfo * info = wxClassInfo::FindClass(type);
	if (!info) return nullptr;
	wxObject * obj = info->CreateObject();
	Filter * filter = wxDynamicCast(obj, Filter);
	if (!filter) wxDELETE(obj);
	return filter;
}

// Keep linker from discarding filters
wxFORCE_LINK_MODULE(AmplifyFilter);
wxFORCE_LINK_MODULE(BackgroundFilter);
wxFORCE_LINK_MODULE(BarrelDistortionCorrectionFilter);
wxFORCE_LINK_MODULE(BrightnessContrastFilter);
wxFORCE_LINK_MODULE(CropFilter);
wxFORCE_LINK_MODULE(DynamicBGFilter);
wxFORCE_LINK_MODULE(EqualizeHistFilter);
wxFORCE_LINK_MODULE(FlipFilter);
wxFORCE_LINK_MODULE(HighpassFilter);
wxFORCE_LINK_MODULE(InvertFilter);
wxFORCE_LINK_MODULE(MonoFilter);
wxFORCE_LINK_MODULE(PerspectiveFilter);
wxFORCE_LINK_MODULE(RectifyFilter);
wxFORCE_LINK_MODULE(ResizeFilter);
wxFORCE_LINK_MODULE(ScalerFilter);
wxFORCE_LINK_MODULE(ShapeFilter);
wxFORCE_LINK_MODULE(SimpleBGFilter);
wxFORCE_LINK_MODULE(SimpleHighpassFilter);
wxFORCE_LINK_MODULE(SmoothingFilter);
wxFORCE_LINK_MODULE(ThresholdFilter);

wxFORCE_LINK_MODULE(KinectBGFilter);
wxFORCE_LINK_MODULE(KinectMonoFilter);
wxFORCE_LINK_MODULE(KinectThresholdFilter);

wxFORCE_LINK_MODULE(DummyFilter);
