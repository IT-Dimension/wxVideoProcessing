#include "stdwx.h"
#include "MotionDetectorBase.h"
#include "MotionDetectorPluginBase.h"
#include "GeometryProvider.h"
#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include <wx/listimpl.cpp>

#include <BlobToJSONFormatter.h>

#if defined(USE_VLD)
#include <vld.h>
#endif

IMPLEMENT_ABSTRACT_CLASS(MotionDetectorBase, SerializableBase);

WX_DEFINE_USER_EXPORTED_LIST(MotionDetectorBaseList);

MotionDetectorBase::MotionDetectorBase()
: m_MotionDetector(nullptr)
, m_CalibrationStep(0)
, m_CalibrationCameraID(0)
, m_bShowCalibrationBlobs(false)
, m_bServiceMode(false)
, m_GeometryProvider(std::move(GeometryProvider(nullptr, nullptr, nullptr)))
{
}

MotionDetectorBase::MotionDetectorBase(MotionDetectorPluginBase * owner, GeometryProvider && geometryProvider)
: m_MotionDetector(owner)
, m_GeometryProvider(std::move(geometryProvider))
, m_CalibrationCameraID(0)
, m_bServiceMode(false)
{
}

MotionDetectorBase::~MotionDetectorBase()
{
}

void MotionDetectorBase::SetServiceMode(bool mode)
{
	m_bServiceMode = mode;
}

MotionDetectorPluginBase * MotionDetectorBase::GetMotionDetector()
{
	return m_MotionDetector;
}

bool MotionDetectorBase::StartCalibration()
{
	return true;
}

bool MotionDetectorBase::StopCalibration()
{
	return true;
}

void MotionDetectorBase::ConfimCalibrationStep(int code)
{

}

bool MotionDetectorBase::IsNextStepCalibrationEnabled()
{
	return true;
}

void MotionDetectorBase::SetCalibrationCameraID(const int index)
{
	m_CalibrationCameraID = index;
}

void MotionDetectorBase::StartDetection()
{
	
}

void MotionDetectorBase::StopDetection()
{
	
}

int MotionDetectorBase::GetBlobs(iFloorBlobVector * blobs)
{
	return 0;
}

void MotionDetectorBase::SetCameraSettings(const IFloorCameraSettings & settings, const int index)
{
	
}

bool MotionDetectorBase::IsServiceMode()
{
	return m_bServiceMode;
}

size_t MotionDetectorBase::GetSupportedCamerasCount()
{
	return m_GeometryProvider.GetSupportedCameraCount();
}

const GeometryProvider & MotionDetectorBase::GetGeometryProvider() const
{
	return m_GeometryProvider;
}
