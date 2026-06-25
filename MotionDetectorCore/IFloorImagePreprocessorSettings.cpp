#include "stdwx.h"
#include "IFloorImagePreprocessorSettings.h"

XS_IMPLEMENT_CLONABLE_CLASS(IFloorImagePreprocessorSettings, xsSerializable);

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(IFloorImagePreprocessorSettingsArray);

#if defined(USE_VLD)
#include <vld.h>
#endif

IFloorImagePreprocessorSettings::IFloorImagePreprocessorSettings()
{
	InitSerialization();
}

IFloorImagePreprocessorSettings::IFloorImagePreprocessorSettings(const IFloorImagePreprocessorSettings & obj)
{
	InitSerialization();
	CopyFrom(obj);
}

IFloorImagePreprocessorSettings& IFloorImagePreprocessorSettings::operator=(const IFloorImagePreprocessorSettings & obj)
{
	if (&obj != this)
	{
		CopyFrom(obj);
	}
	return *this;
}

IFloorImagePreprocessorSettings::~IFloorImagePreprocessorSettings()
{
}

void IFloorImagePreprocessorSettings::InitSerialization()
{
	XS_SERIALIZE(m_Filters, wxT("Filters"));
}

void IFloorImagePreprocessorSettings::CopyFrom(const IFloorImagePreprocessorSettings & obj)
{
	m_Filters = obj.m_Filters;
}

wxString IFloorImagePreprocessorSettings::GetFilterConfig(const wxString & filterName)
{
	StringMap::iterator it = m_Filters.find(filterName);
	if (it == m_Filters.end())
		return wxEmptyString;
	return it->second;
}

wxString IFloorImagePreprocessorSettings::operator[](const wxString & filterName)
{
	return GetFilterConfig(filterName);
}

void IFloorImagePreprocessorSettings::SetFilterConfig(const wxString & filterName, const wxString & config)
{
	m_Filters[filterName] = config;
}