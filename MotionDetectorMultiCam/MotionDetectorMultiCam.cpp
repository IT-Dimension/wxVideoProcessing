#include "stdwx.h"
#include "MotionDetectorMultiCam.h"
#include "MultiCamSettingsPanel.h"
#include "MultiCamCameraManager.h"
#include "ContourFinder.h"
#include "Tracking.h"

#include <GeometryProvider.h>
#include "PerspectiveFilter.h"
#include "FlipFilter.h"

#if defined(USE_VLD)
#include <vld.h>
#endif

const wxString MotionDetectorMultiCam::DEFAULT_FILTERS[] =
{
	wxT("DummyFilter"),
	//wxT("EqualizeHistFilter"),
	wxT("BrightnessContrastFilter"),
	wxT("MonoFilter"),
	wxT("FlipFilter"),
	wxT("SimpleBGFilter"),
	wxT("DynamicBGFilter"),
	wxT("SubOrBGFilter"),
	wxT("XorBGFilter"),
	wxT("PerspectiveFilter"),
	wxT("SmoothingFilter"),
	wxT("SimpleHighpassFilter"),
	wxT("AmplifyFilter"),
	wxT("InvertFilter"),
	wxT("ThresholdFilter"),
};

IMPLEMENT_DYNAMIC_CLASS(MotionDetectorMultiCam, MotionDetectorBase);

MotionDetectorMultiCam::MotionDetectorMultiCam()
: MotionDetectorBase(nullptr, GeometryProvider(nullptr, nullptr, nullptr))
, m_CameraManager(NULL)
, m_ContourFinder(NULL)
, m_BlobTracker(NULL)
, m_minBlobArea(100), m_maxBlobArea(1000000), m_maxBlobCount(5), m_bUseOneOnMulti(false)
{
	wxLogDebug(wxT("Size of MotionDetectorMultiCam : %i"), (int)sizeof(MotionDetectorMultiCam));
	m_Settings.DeleteContents(true);
	//m_hash = 8180;
	//m_hash = 18540; //cpu1
	//m_hash = 23132; //cpu2
}

MotionDetectorMultiCam::MotionDetectorMultiCam(MotionDetectorPluginBase * owner, 
	GeometryProvider && geometryProvider)
: MotionDetectorBase(owner, std::move(geometryProvider))
, m_CameraManager(NULL)
, m_ContourFinder(NULL)
, m_BlobTracker(NULL)
, m_minBlobArea(100), m_maxBlobArea(1000000), m_maxBlobCount(5), m_bUseOneOnMulti(false)
{
	wxLogDebug(wxT("Size of MotionDetectorMultiCam : %i"), (int)sizeof(MotionDetectorMultiCam));
	wxLogDebug(wxT("Size of IFLoorImageProcessorSettings : %i"), (int)sizeof(IFloorImagePreprocessorSettings));
	XS_SERIALIZE(m_minBlobArea, wxT("minBlobArea"));
	XS_SERIALIZE(m_maxBlobArea, wxT("maxBlobArea"));
	XS_SERIALIZE(m_maxBlobCount, wxT("maxBlobCount"));
	XS_SERIALIZE(m_bUseOneOnMulti, wxT("useOneOnMulti"));
	m_Settings.DeleteContents(true);
	//m_hash = 8180;
	//m_hash = 18540; //cpu1
	//m_hash = 23132; //cpu2
}

MotionDetectorMultiCam::~MotionDetectorMultiCam()
{
	Clear();
	m_Settings.Clear();
	m_Buffers.Clear();
	m_BufferSizes.Clear();
	m_CameraSettings.Clear();
	//StopDetection();
}

wxString MotionDetectorMultiCam::GetName() const
{
	static wxString id = wxT("MultiCam detector");
	return id;
}

wxString MotionDetectorMultiCam::GetID() const
{
	static wxString id = wxT("{BFED0460-5C8D-4114-861B-96DE3FE9C607}");
	return id;
}

void MotionDetectorMultiCam::StartDetection()
{
	Clear();

	if (m_Settings.IsEmpty()) // wxT("There is no any camera settings")
		return;

	m_CameraManager = new MultiCamCameraManager(m_GeometryProvider);

	m_ContourFinder = new ContourFinder();
	m_BlobTracker = new BlobTracker();

	size_t index = 0;

	m_CameraManager->SetUseOneOnMulti(m_bUseOneOnMulti);
	
	for (IFloorCameraSettingsList::iterator it = m_Settings.begin(); it != m_Settings.end(); ++it, ++index)
	{
		m_CameraManager->AddCamera(*it, m_Buffers[index], m_BufferSizes[index]);
		// Add filters for the camera
		bool hasConfig = index < m_CameraSettings.Count();
		for (size_t i = 0; i < WXSIZEOF(DEFAULT_FILTERS); ++i)
			m_CameraManager->AddFilter(index, DEFAULT_FILTERS[i], hasConfig ? m_CameraSettings[index][DEFAULT_FILTERS[i]] : wxString(wxEmptyString));

		IFloorImagePreprocessor * camera = m_CameraManager->GetCameraByID(index);

		PerspectiveFilter * pf = wxDynamicCast(camera->GetFilter(wxT("PerspectiveFilter")), PerspectiveFilter);
		if (pf)
			pf->SetSettings(**it);

		FlipFilter * ff = wxDynamicCast(camera->GetFilter(wxT("FlipFilter")), FlipFilter);
		if (ff)
			ff->SetSettings(**it);
	}
	m_CameraManager->InitCameraParams();
	m_CameraManager->CreateFullView();
}

void MotionDetectorMultiCam::StopDetection()
{
	Clear();
	m_Settings.Clear();
	m_Buffers.Clear();
	m_BufferSizes.Clear();
}

IFloorImagePreprocessor * MotionDetectorMultiCam::GetCameraImagePreprocessor(int cameraID)
{
	if (!m_CameraManager)
		return NULL;
	return m_CameraManager->GetCameraByID(cameraID);
}

int MotionDetectorMultiCam::GetBlobs(iFloorBlobVector * blobs)
{
	if (m_ContourFinder)
	{
		if (m_BlobTracker){
			*blobs = m_BlobTracker->getTrackedBlobs();
		}else{
			*blobs = m_ContourFinder->blobs;
		}
		for(size_t i = 0; i < blobs->size(); ++i){
			iFloorBlob& blob = (*blobs)[i];
			//blob.centroid.x *= m_hash;
			//blob.centroid.y /= m_hash;
		}
	}
	return 0;
}

bool MotionDetectorMultiCam::Calibrate()
{
	return true;
}

CommonConfigWindowBase * MotionDetectorMultiCam::CreateSettingsEditor(wxWindow * parent)
{
	return NULL;
}

void MotionDetectorMultiCam::SetCameraSettings(const IFloorCameraSettings & settings, const int index)
{
	wxASSERT(index >= 0);
	IFloorCameraSettings * newSettings = new IFloorCameraSettings(settings);
	while (index > (int)m_Settings.GetCount())
	{
		m_Settings.Append(new IFloorCameraSettings); // Add dummy settings to the list
	}
	if (index == m_Settings.GetCount())
		m_Settings.Append(newSettings);
	else
		m_Settings.Item(index)->SetData(newSettings);

	m_Buffers.SetCount(m_Settings.GetCount());
	m_BufferSizes.SetCount(m_Settings.GetCount());
}

void MotionDetectorMultiCam::SetDataBuffer(unsigned char * buffer, const size_t length, const int index)
{
	wxCHECK(index < (int)m_Buffers.GetCount(), );
	m_Buffers[index] = buffer;
	m_BufferSizes[index] = length;
}

void MotionDetectorMultiCam::ProcessData()
{
	wxCHECK_MSG(m_CameraManager && m_ContourFinder && m_BlobTracker, , wxT("You should call StartDetection() first"));

	cv::Mat image = m_CameraManager->GetImage();
	if (image.empty())
		return;

	m_ContourFinder->findContours(image, m_minBlobArea, m_maxBlobArea, m_maxBlobCount, false);
	if (m_BlobTracker){
		m_BlobTracker->track(m_ContourFinder);
	}
}

bool MotionDetectorMultiCam::Deserialize(wxInputStream & instream)
{
	bool res = SerializableBase::Deserialize(instream, *this);
	if (res)
	{
		m_CameraSettings.Clear();
		xsSerializable * child;
		while (child = xsSerializable::GetFirstChild(CLASSINFO(IFloorImagePreprocessorSettings)))
		{
			child->Reparent(NULL);
			child->SetParentManager(NULL);
			m_CameraSettings.Add(wxDynamicCast(child, IFloorImagePreprocessorSettings));
		}
	}
	return res;
}

bool MotionDetectorMultiCam::Serialize(wxOutputStream & outstream)
{
	for (size_t i = 0; i < m_CameraSettings.Count(); ++i)
	{
		xsSerializable::AddChild(&m_CameraSettings[i]);
	}
	bool res = SerializableBase::Serialize(outstream, *this);

	xsSerializable * child;
	while ((child = xsSerializable::GetFirstChild(CLASSINFO(IFloorImagePreprocessorSettings))))
	{
		child->Reparent(NULL);
		child->SetParentManager(NULL);
	}
	return res;
}

void MotionDetectorMultiCam::Clear()
{
	wxDELETE(m_BlobTracker);
	wxDELETE(m_ContourFinder);
	wxDELETE(m_CameraManager);
}

IFloorImagePreprocessorSettingsArray & MotionDetectorMultiCam::GetCameraSettings()
{
	return m_CameraSettings;
}

bool MotionDetectorMultiCam::IsRunning()
{
	return m_CameraManager != NULL;
}
