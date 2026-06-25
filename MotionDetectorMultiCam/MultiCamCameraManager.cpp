#include "stdwx.h"
#include <GeometryProvider.h>
#include "MultiCamCameraManager.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#if defined(USE_VLD)
#include <vld.h>
#endif

#define CMAN_ERR_NO_ERROR	0
#define CMAN_ERR_NO_INDEX	2
#define CMAN_ERR_WRONG_SIZE 3
#define CMAN_ERR_NO_FILTER	4

const int MultiCamCameraManager::MAX_WIDTH  = INT_MAX;
const int MultiCamCameraManager::MAX_HEIGHT = INT_MAX;
const wxString MultiCamCameraManager::ERROR_MESSAGES[] =
{
	_("No error"),
	_("The depth or the number of channels differ in cameras images"),
	_("There is no camera with such index"),
	_("Wrong height or width of the full view image"),
	_("There is no filter with such name"),
};

MultiCamCameraManager::MultiCamCameraManager(GeometryProvider & geometryProvider)
	: m_Height(MAX_WIDTH)
	, m_Width(MAX_HEIGHT)
	, m_LastError(0)
	, m_bUseOneOnMulti(false)
	, m_GeometryProvider(geometryProvider)
{
	//cvNamedWindow("1");
}

MultiCamCameraManager::~MultiCamCameraManager(void)
{
	//cvDestroyWindow("1");
	CleanDisplayImages();

	for (IFloorImagePreprocessorVector::iterator it = m_ArrayOfCameras.begin(); it != m_ArrayOfCameras.end(); ++it)
	{
		delete *it;
	}
	m_ArrayOfCameras.clear();
}

IFloorImagePreprocessor * MultiCamCameraManager::GetCameraByID(int cameraID)
{
	if (cameraID < (int)m_ArrayOfCameras.size() )
		return m_ArrayOfCameras[cameraID];
	else
		return nullptr;
}

void MultiCamCameraManager::SetUseOneOnMulti(bool useOne)
{
	m_bUseOneOnMulti = useOne;
}

void MultiCamCameraManager::AddCamera(const IFloorCameraSettings * settings,  void * buffer, size_t bufLength)
{
	m_ArrayOfCameras.push_back(new IFloorImagePreprocessor(settings, buffer, bufLength));
}

bool MultiCamCameraManager::InitCameraParams()
{
	// Look for minimum sizes
	for (size_t i = 0; i < m_ArrayOfCameras.size(); i++)
	{
		IFloorImagePreprocessor * capture = m_ArrayOfCameras[i];
		if (capture != nullptr)
		{
			m_Height = wxMin(capture->GetSettings()->CamHeight, m_Height);
			m_Width = wxMin(capture->GetSettings()->CamWidth, m_Width);
		}
	}

	// Decided what capture we must resize
	for (size_t i = 0; i < m_ArrayOfCameras.size(); i++)
	{
		IFloorImagePreprocessor * capture = m_ArrayOfCameras[i];
		if (capture != nullptr)
		{
			capture->PrepareImage(m_Width, m_Height);
		}
	}
	return (m_Height != MAX_HEIGHT) || (m_Width != MAX_WIDTH);
}

cv::Mat MultiCamCameraManager::GetImage()
{
	wxPoint pointIn;
	wxRectVector displays;
	m_GeometryProvider.GetGeometry(displays);
	size_t count = wxMin(displays.size(), m_ArrayOfCameras.size());

	if (m_ImgFullView.empty() && count > 0)
	{
		m_ImgFullView = cv::Mat::zeros(cv::Size(m_Width, m_Height), CV_8UC3);
	}

	for (auto i = 0; i < count; ++i)
	{
		IFloorImagePreprocessor * capture = m_ArrayOfCameras[i];
		if (capture != nullptr)
		{
			wxRect & display = displays[i];
			cv::Mat img = capture->ProcessImage();
			if (img.empty())
				continue;
			
			// Create image if necessary 
			if (m_DispImages.size() < i + 1)
			{
				m_DispImages.push_back(cv::Mat::zeros(cv::Size(display.GetWidth(), display.GetHeight()), img.type()));
			}

			if (m_bUseOneOnMulti)
			{
				cv::resize(img, m_ImgFullView, m_ImgFullView.size());
			}
			else // If we use default system monitors geometry and one camera per monitor
			{

				// Resize image to fit display size
				cv::Mat displayImg = m_DispImages[i];
				cv::resize(img, displayImg, displayImg.size());
				// Calculate insertion point
				pointIn = display.GetLeftTop();

				pointIn -= m_LeftTop;

				cv::Rect roi(pointIn.x, pointIn.y, displayImg.cols, displayImg.rows);

				if (roi.x >= 0 && roi.y >= 0 && (roi.x + roi.width) <= m_ImgFullView.cols && (roi.y + roi.height) <= m_ImgFullView.rows)
				{
					displayImg.copyTo(m_ImgFullView(roi));
				}
			}
		}
	}
	/*if(rand() % 20 == 0) 
	{
		cvSaveImage("test.bmp", m_ImgFullView);
		IplImage * t = cvCreateImage(cvSize(m_ImgFullView->width*0.3, m_ImgFullView->height*0.3), m_ImgFullView->depth, m_ImgFullView->nChannels);
		cvResize(m_ImgFullView, t);
		cvShowImage("1", t);
		cvReleaseImage(&t);
	}*/
	
	return m_ImgFullView;
}


bool MultiCamCameraManager::SetROI(int x, int y, int width, int height, int cameraID)
{
	
	if (cameraID < (int)m_ArrayOfCameras.size())
	{
		m_ArrayOfCameras[cameraID]->SetROI(x, y, width, height);
		m_LastError = CMAN_ERR_NO_ERROR;
		return true;
	}
	else{
		m_LastError = CMAN_ERR_NO_INDEX;
		return false;
	}
}

bool MultiCamCameraManager::CreateFullView()
{
	wxRect workingArea;

	CleanDisplayImages();
	wxRectVector displays;
	m_GeometryProvider.GetGeometry(displays);
	for (auto i = 0; i < displays.size(); ++i)
	{
		wxRect & display = displays[i];
		workingArea.Union(display);
	}
	m_Height = workingArea.height;
	m_Width = workingArea.width;
	m_LeftTop = workingArea.GetLeftTop();
	m_ImgFullView.release();

	return true;
}

wxString MultiCamCameraManager::GetLastError()
{
	return ERROR_MESSAGES[m_LastError];
}

bool MultiCamCameraManager::AddFilter(int cameraID, const wxString & filterType, const wxString & config /*= wxEmptyString*/)
{
	if (cameraID < (int)m_ArrayOfCameras.size())
	{
		if (m_ArrayOfCameras[cameraID]->AddFilterToChain(filterType, config))
		{
			m_LastError = CMAN_ERR_NO_ERROR;
			return true;
		}
		else
		{
			m_LastError = CMAN_ERR_NO_FILTER;
		}
	}
	else
	{
		m_LastError = CMAN_ERR_NO_INDEX;
	}
	return false;
}

bool MultiCamCameraManager::SetFilterSettings(int cameraID, const wxString & filterType, const wxString & config)
{
	IFloorImagePreprocessor * camera = GetCameraByID(cameraID);
	if (!camera)
		return false;

	return camera->SetFilterSettings(filterType, config);
}

void MultiCamCameraManager::CleanDisplayImages()
{
	for (size_t i=0; i < m_DispImages.size(); i++)
	{
		m_DispImages[i].release();
	}	
	m_DispImages.clear();
}
