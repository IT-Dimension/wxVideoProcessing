#include "stdwx.h"
#include "MotionDetectorPluginBase.h"
#include "MotionDetectorBase.h"

IMPLEMENT_ABSTRACT_CLASS(MotionDetectorPluginBase, wxObject)

MotionDetectorPluginBase::~MotionDetectorPluginBase()
{
}

void MotionDetectorPluginBase::DeleteDetector(MotionDetectorBase * detector)
{
	wxDELETE(detector);
}
