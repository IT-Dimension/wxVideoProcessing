#ifndef _MOTIONDETECTORBASE_H
#define _MOTIONDETECTORBASE_H

#include "MotionDetectorPlugin.h"
#include <iFloorBlob.h>
#include <SerializableBase.h>
#include <map>

typedef std::map<std::string, std::string> MotionDetectorParametersMap;

class MotionDetectorPluginBase;
class IFloorCameraSettings;
class MotionDetectorBase;
class GeometryProvider;

class IFLOOR_API MotionDetectorBase : public SerializableBase
{

	DECLARE_ABSTRACT_CLASS(MotionDetectorBase);
public:
	MotionDetectorBase();
	MotionDetectorBase(MotionDetectorPluginBase * owner, GeometryProvider && geometryProvider);
	virtual ~MotionDetectorBase();
	virtual void SetServiceMode(bool mode = true);
//Calibration API
	virtual bool Calibrate() = 0;
	virtual void SetCalibrationCameraID(const int index);

	virtual bool StartCalibration();
	virtual bool StopCalibration();
	unsigned int GetCalibrationStep() {return m_CalibrationStep;}
	virtual void ConfimCalibrationStep(int code);
	virtual bool IsNextStepCalibrationEnabled();
	bool IsCalibrationBlobsShowed(){return m_bShowCalibrationBlobs;}
//Common detection API
	virtual void StartDetection();
	virtual void StopDetection();
	virtual void SetDataBuffer(unsigned char * buffer, const size_t length, const int index) = 0;
	virtual void ProcessData() = 0;
	// Returns how many cameras supports this detector
	virtual size_t GetSupportedCamerasCount();
	virtual int GetBlobs(iFloorBlobVector * blobs);
	virtual void SetCameraSettings(const IFloorCameraSettings & settings, const int index);
	const GeometryProvider & GetGeometryProvider() const;

	//Service mode control
	bool IsServiceMode();
	bool IsJSONmsgsMode();
	bool IsSendBlobsJmsgMode();
	MotionDetectorPluginBase * GetMotionDetector();

protected:
	MotionDetectorPluginBase * m_MotionDetector;
	GeometryProvider && m_GeometryProvider;
	unsigned int m_CalibrationStep;
	int m_CalibrationCameraID;
	bool m_bShowCalibrationBlobs;
	bool m_bServiceMode;
};



WX_DECLARE_USER_EXPORTED_LIST(MotionDetectorBase, MotionDetectorBaseList, IFLOOR_API);

typedef MotionDetectorPluginBase * (*CreateMotionDetectorPlugin_function)();
typedef void (*DeleteMotionDetectorPlugin_function)(MotionDetectorPluginBase * plugin);

#endif // _MOTIONDETECTORBASE_H