#ifndef _MULTICAMCAMERAMANAGER_H
#define _MULTICAMCAMERAMANAGER_H
#include <opencv2/opencv.hpp>
#include <IFloorImagePreprocessor.h>

// Type definition for display image vector
typedef std::vector<cv::Mat> ImageVector;

class GeometryProvider;

class MultiCamCameraManager
{
public:
	MultiCamCameraManager(GeometryProvider & geometryProvider);
	~MultiCamCameraManager(void);
	// Add camera reference
	void AddCamera(const IFloorCameraSettings * settings,  void * buffer, size_t bufLength);
	// Calculate camera view
	bool InitCameraParams();
	// Set the region of interest for camera with specific ID
	bool SetROI(int x, int y, int width, int height, int cameraID);
	// Need to create a full camera view after all initializations
	bool CreateFullView();
	// Get last error msg
	wxString GetLastError();
	// Add a filter to the camera
	bool AddFilter(int cameraID, const wxString & filterType, const wxString & config = wxEmptyString);
	// Change filter settings
	bool SetFilterSettings(int cameraID, const wxString & filterType, const wxString & config);
	// Get concatenate view
	cv::Mat GetImage();
	// Clean the display image vector
	void CleanDisplayImages();
	// Get camera filter preprocessor by ID
	IFloorImagePreprocessor * GetCameraByID(int cameraID);
	// Use one sensor for all monitors
	void SetUseOneOnMulti(bool useOne);

	bool IsUsingCustomGeometry();
protected:
	// Cameras map
	IFloorImagePreprocessorVector m_ArrayOfCameras;
	// Full camera view
	cv::Mat m_ImgFullView;
	// Display image vector
	ImageVector m_DispImages;
	// Base image size
	int m_Height;
	int m_Width;
	// Arrays of heights and widths
	wxArrayInt m_RowHeights;
	wxArrayInt m_ColWidths;
	// Arrays of offsets
	wxArrayInt m_OffsetsX;
	wxArrayInt m_OffsetsY;
	// Last error number
	int m_LastError;
	// Left-top point of union display rectangle
	wxPoint  m_LeftTop;
	// Use one sensor for all monitors
	bool m_bUseOneOnMulti;
	
	GeometryProvider & m_GeometryProvider;
private:
	static const int MAX_HEIGHT;
	static const int MAX_WIDTH;
	static const wxString ERROR_MESSAGES[];
};

#endif // _MULTICAMCAMERAMANAGER_H