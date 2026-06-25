#ifndef IFLOORIMAGEPREPROCESSOR_H
#define IFLOORIMAGEPREPROCESSOR_H

#include <opencv2/opencv.hpp>
#include "FilterFactory.h"
#include <map>
#include <IFloorCameraSettings.h>
#include <vector>

typedef std::vector<Filter*> ChainOfFilters;

class MOTION_DETECTOR_CORE_API IFloorImagePreprocessor
{
public:
	IFloorImagePreprocessor(const IFloorCameraSettings * settings, void * buffer, size_t bufLength);
	~IFloorImagePreprocessor();
	// Create an image for resizing
	void PrepareImage(int nWidth, int nHeight);
	// Capture image and process all filters
	cv::Mat ProcessImage();
	// Add a new filter to chain
	bool AddFilterToChain(const wxString & filterType, const wxString & config = wxEmptyString);
	// Returns current camera settings
	const IFloorCameraSettings * GetSettings() const;
	// Set the region of interest for the camera
	bool SetROI(int x, int y, int width, int height);
	// Return size of roi if it exists or size of the image otherwise
	cv::Size GetImageSize() const;
	// Returns filter from the chain with the specified type
	Filter * GetFilter(const wxString & filterType);
	// Deserializes filter in chain with specified type from the config string
	bool SetFilterSettings(const wxString & filterType, const wxString & config);
	// Deserializes filter from the config string
	static bool SetFilterSettings(Filter * filter, const wxString & config);

	ChainOfFilters & GetFilters();
protected:
	void QueryImage();

protected:
	const IFloorCameraSettings * m_Settings;
	cv::Mat m_ImgCamera;
	cv::Mat m_ImgFlash;
	cv::Rect m_Roi;          
	bool m_bHasRoi;

	ChainOfFilters m_Filters;
	bool m_bNeedToResize;
	void * m_pBuffer;
	size_t m_BufLength;
};

// Type definition for cameras vector
typedef std::vector<IFloorImagePreprocessor*> IFloorImagePreprocessorVector;

#endif //IFLOORIMAGEPREPROCESSOR_H