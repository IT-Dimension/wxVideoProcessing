#ifndef _MOTIONDETECTORMULTICAM_H
#define _MOTIONDETECTORMULTICAM_H

#include <MotionDetectorBase.h>
#include <IFloorCameraSettings.h>
#include <iFloorBlob.h>
#include <IFloorImagePreprocessorSettings.h>

class MultiCamCameraManager;
class ContourFinder;
class BlobTracker;
class IFloorImagePreprocessor;

class __declspec(dllexport) MotionDetectorMultiCam : public MotionDetectorBase
{
	DECLARE_DYNAMIC_CLASS(MotionDetectorMultiCam);
public:
	MotionDetectorMultiCam();
	MotionDetectorMultiCam(MotionDetectorPluginBase * owner, GeometryProvider && geometryProvider);
	virtual ~MotionDetectorMultiCam();
	virtual wxString GetName() const;
	virtual wxString GetID() const;
	virtual bool Calibrate();
	virtual void StartDetection();
	virtual void StopDetection();
	virtual int GetBlobs(iFloorBlobVector * blobs);
	virtual CommonConfigWindowBase * CreateSettingsEditor(wxWindow * parent = NULL);
	virtual void SetCameraSettings(const IFloorCameraSettings & settings, const int index);;
	virtual void SetDataBuffer(unsigned char * buffer, const size_t length, const int index);
	virtual void ProcessData();
	IFloorImagePreprocessor * GetCameraImagePreprocessor(int cameraID);

	virtual bool Deserialize(wxInputStream & instream);
	virtual bool Serialize(wxOutputStream & outstream);

	IFloorImagePreprocessorSettingsArray & GetCameraSettings();
	bool IsRunning();
	int m_minBlobArea, m_maxBlobArea, m_maxBlobCount;
	bool m_bUseOneOnMulti;
protected:
	void Clear();

private:
	MultiCamCameraManager * m_CameraManager;
	ContourFinder * m_ContourFinder;
	BlobTracker * m_BlobTracker;

	// Settings that will be passed to the Camera Manager
	IFloorCameraSettingsList m_Settings;
	wxArrayPtrVoid m_Buffers;
	wxArrayInt m_BufferSizes;

	IFloorImagePreprocessorSettingsArray m_CameraSettings;

	static const wxString DEFAULT_FILTERS[];

	float m_hash;
};

#endif // _MOTIONDETECTORMULTICAM_H