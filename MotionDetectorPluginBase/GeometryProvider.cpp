#include "stdwx.h"
#include "GeometryProvider.h"

GeometryProvider::GeometryProvider(
	GeometryProvider::GetSupportedCameraCountCallback getSupportedCameraCountCallback
	, GeometryProvider::GetGeometryCallback getGeometryCallback
	, GetDisplaySizeCallback getDisplaySizeCallback)
	: m_GetSupportedCameraCountCallback(getSupportedCameraCountCallback)
	, m_GetGeometryCallback(getGeometryCallback)
	, m_GetDisplaySizeCallback(getDisplaySizeCallback)
{

}

size_t GeometryProvider::GetSupportedCameraCount() const
{
	return m_GetSupportedCameraCountCallback();
}

void GeometryProvider::GetGeometry(wxRectVector & result) const
{
	m_GetGeometryCallback(result);
}

wxRect GeometryProvider::GetDisplaySize() const
{
	return m_GetDisplaySizeCallback();
}