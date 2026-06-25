#include "stdwx.h"
#include "MotionDetectorConfigWindowBase.h"

IMPLEMENT_DYNAMIC_CLASS(MotionDetectorConfigWindowBase, CommonConfigWindowBase)


MotionDetectorConfigWindowBase::MotionDetectorConfigWindowBase()
: m_Detector(NULL), m_Started(false)
{
}

MotionDetectorConfigWindowBase::MotionDetectorConfigWindowBase(MotionDetectorBase * detector, wxWindow * parent)
{
	Create(detector, parent);
}

bool MotionDetectorConfigWindowBase::Create(MotionDetectorBase * detector, wxWindow * parent)
{
	m_Detector = detector;
    m_Started = false;
	return CommonConfigWindowBase::Create(parent);
}

MotionDetectorConfigWindowBase::~MotionDetectorConfigWindowBase(void)
{
}

bool MotionDetectorConfigWindowBase::ReadConfig()
{
	return false;
}

bool MotionDetectorConfigWindowBase::SaveConfig()
{
	return false;
}

void MotionDetectorConfigWindowBase::StartDetection()
{
    m_Started = true;
}
void MotionDetectorConfigWindowBase::StopDetection()
{
    m_Started = false;
}

MotionDetectorBase * MotionDetectorConfigWindowBase::GetDetector()
{
	return m_Detector;
}