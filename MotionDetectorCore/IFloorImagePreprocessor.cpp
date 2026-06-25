#include "stdwx.h"
#include "IFloorImagePreprocessor.h"

#if defined(USE_VLD)
#include <vld.h>
#endif

IFloorImagePreprocessor::IFloorImagePreprocessor(const IFloorCameraSettings * settings, void * buffer, size_t bufLength)
	: m_Settings(settings)
	, m_bNeedToResize(false)
	, m_pBuffer(buffer)
	, m_BufLength(bufLength)
	, m_bHasRoi(false)
{
	if (m_pBuffer && m_Settings)
	{
		int numChannels = bufLength / (settings->CamWidth * settings->CamHeight);

		int type;

		if (numChannels == 4)
		{
			type = CV_8UC4;
		}
		else
		{
			if (numChannels == 3)
			{
				type = CV_8UC3;
			}
			else
			{
				type = CV_8UC1;
			}
		}
		m_ImgCamera = cv::Mat(settings->CamHeight, settings->CamWidth, type, m_pBuffer);
	}
}

IFloorImagePreprocessor::~IFloorImagePreprocessor()
{
	wxYield();
	for (auto* filter : m_Filters)
	{
		delete filter;
	}
	m_Filters.clear();
}

void IFloorImagePreprocessor::QueryImage()
{
	if (m_bNeedToResize && !m_ImgCamera.empty())
	{
		cv::resize(m_ImgCamera, m_ImgFlash, m_ImgFlash.size(), 0, 0, cv::INTER_LINEAR);
	}
}

void IFloorImagePreprocessor::PrepareImage(int nWidth, int nHeight)
{
	m_bNeedToResize = (nHeight != m_Settings->CamHeight) || (nWidth != m_Settings->CamWidth);
	// If the image needs to resize then create destination image
	if (m_bNeedToResize && !m_ImgCamera.empty())
	{
		m_ImgFlash = cv::Mat::zeros(nHeight, nWidth, m_ImgCamera.type());
	}
	else
	{
		m_ImgFlash = m_ImgCamera;
	}
}

cv::Mat IFloorImagePreprocessor::ProcessImage()
{
	// Get an image from camera
	QueryImage();
	// Apply a chain of filters
	if (m_ImgFlash.empty())
		return cv::Mat();

	cv::Mat currentFrame = m_bHasRoi ? m_ImgFlash(m_Roi) : m_ImgFlash;

	cv::Mat result = currentFrame;

	if (!m_Filters.empty())
	{
		Filter* firstFilter = m_Filters.front();
		result = firstFilter->Process(currentFrame);
	}

	return result;
}

bool IFloorImagePreprocessor::AddFilterToChain(const wxString & filterType, const wxString & config /*= wxEmptyString*/)
{
	Filter * f = FilterFactory::CreateFilter(filterType);
	if (f != nullptr)
	{
		SetFilterSettings(f, config);
		if (!m_Filters.empty())
			m_Filters.back()->ConnectTo(f);
		m_Filters.push_back(f);
		return true;
	}

	return false;
}

const IFloorCameraSettings * IFloorImagePreprocessor::GetSettings() const
{
	return m_Settings;
}

bool IFloorImagePreprocessor::SetROI(int x, int y, int width, int height)
{
	if (m_ImgFlash.empty())
		return false;

	x = std::max(0, std::min(x, m_ImgFlash.cols));
	y = std::max(0, std::min(y, m_ImgFlash.rows));
	width = std::max(0, std::min(width, m_ImgFlash.cols - x));
	height = std::max(0, std::min(height, m_ImgFlash.rows - y));

	// Check and correct coords and size
	if (width > 0 && height > 0)
	{
		m_Roi = cv::Rect(x, y, width, height);
		m_bHasRoi = true;
		return true;
	}
	
	m_bHasRoi = false;
	return false;
}

cv::Size IFloorImagePreprocessor::GetImageSize() const
{
	if (m_bHasRoi)
	{
		return m_Roi.size();
	}
	if (!m_ImgFlash.empty())
	{
		return m_ImgFlash.size();
	}
	return cv::Size(m_Settings->CamWidth, m_Settings->CamHeight);
}

ChainOfFilters & IFloorImagePreprocessor::GetFilters()
{
	return m_Filters;
}

Filter * IFloorImagePreprocessor::GetFilter(const wxString & filterType)
{
	if(!m_Filters.empty()){
		auto end = m_Filters.end();
		for (auto it = m_Filters.begin(); it != end; ++it)
		{
			Filter * filter = *it;
			if (filter->GetClassInfo()->GetClassName() == filterType)
				return filter;
		}
	}
	return nullptr;
}

bool IFloorImagePreprocessor::SetFilterSettings(const wxString & filterType, const wxString & config)
{
	Filter * filter = GetFilter(filterType);
	return SetFilterSettings(filter, config);
}

bool IFloorImagePreprocessor::SetFilterSettings(Filter * filter, const wxString & config)
{
	if (!filter || config.IsEmpty())
		return false;
	return SerializableBase::Deserialize(config, *filter);
}