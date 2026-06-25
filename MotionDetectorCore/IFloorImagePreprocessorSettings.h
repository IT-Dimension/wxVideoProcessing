#ifndef _IFLOORIMAGEPREPROCESSORSETTINGS_H
#define _IFLOORIMAGEPREPROCESSORSETTINGS_H

#include <wx/wxxmlserializer/XmlSerializer.h>
#include "MotionDetectorCorePlugin.h"

class MOTION_DETECTOR_CORE_API IFloorImagePreprocessorSettings : public xsSerializable
{
	XS_DECLARE_CLONABLE_CLASS(IFloorImagePreprocessorSettings);
public:
	IFloorImagePreprocessorSettings();
	IFloorImagePreprocessorSettings(const IFloorImagePreprocessorSettings & obj);
	IFloorImagePreprocessorSettings& operator=(const IFloorImagePreprocessorSettings & obj);
	virtual ~IFloorImagePreprocessorSettings();

	wxString GetFilterConfig(const wxString & filterName);
	void SetFilterConfig(const wxString & filterName, const wxString & config);
	wxString operator[](const wxString & filterName);

protected:
	void InitSerialization();
	void CopyFrom(const IFloorImagePreprocessorSettings & obj);
private:
	StringMap m_Filters;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(IFloorImagePreprocessorSettings, IFloorImagePreprocessorSettingsArray, MOTION_DETECTOR_CORE_API);

#endif // _IFLOORIMAGEPREPROCESSORSETTINGS_H
