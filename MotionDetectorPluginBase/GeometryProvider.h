#pragma once

#include "MotionDetectorPlugin.h"

typedef std::vector<wxRect> wxRectVector;

class IFLOOR_API GeometryProvider
{
	
	typedef std::function<size_t()> GetSupportedCameraCountCallback;
	typedef std::function<void(wxRectVector &)> GetGeometryCallback;
	typedef std::function<wxRect()> GetDisplaySizeCallback;
public:
	GeometryProvider(GetSupportedCameraCountCallback getSupportedCameraCountCallback, 
		GetGeometryCallback getGeometryCallback,
		GetDisplaySizeCallback getDisplaySizeCallback);
	
	size_t GetSupportedCameraCount() const;
	void GetGeometry(wxRectVector & result) const;
	wxRect GetDisplaySize() const;
	
private:
	 GetSupportedCameraCountCallback m_GetSupportedCameraCountCallback;
	 GetGeometryCallback m_GetGeometryCallback;
	 GetDisplaySizeCallback m_GetDisplaySizeCallback;
};