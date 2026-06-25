#ifndef _SERIALIZABLEBASE_H
#define _SERIALIZABLEBASE_H

#include "CommonPlugin.h"
#include "CommonConfigWindowBase.h"
#include <wx/wxxmlserializer/XmlSerializer.h>

/// Base class for all iFloor effects
class IFLOOR_API_COMMONPLUGINBASE SerializableBase : public xsSerializable
{
	DECLARE_ABSTRACT_CLASS(SerializableBase)
public:
	/// Default constructor
	SerializableBase();
	/// Default destructor
	virtual ~SerializableBase();
	/// Returns GUID (unique identifier) of effect
	/// \return string which contains unique identifier of effect
	virtual wxString GetID() const = 0;
	/// Returns name of effect
	/// \return string which contains human-readable name of effect
	virtual wxString GetName() const = 0;
	/// Creates settings panel where user can setup effect properties
	/// \param parent parent window of settings panel
	/// \return pointer to created settings panel. If function fails, return value is NULL
	virtual CommonConfigWindowBase * CreateSettingsEditor(wxWindow * parent = NULL) = 0;

	virtual bool Deserialize(wxInputStream & instream);
	virtual bool Serialize(wxOutputStream & outstream);

	static bool Deserialize(wxInputStream & instream, xsSerializable & obj);
	static bool Serialize(wxOutputStream & outstream, xsSerializable & obj);

	static bool Deserialize(const wxString & config, xsSerializable & obj);
	static wxString Serialize(xsSerializable & obj);
};

/// List of iFloor effects
WX_DECLARE_USER_EXPORTED_LIST(SerializableBase, SerializableBaseList, IFLOOR_API_COMMONPLUGINBASE);
WX_DECLARE_STRING_HASH_MAP(SerializableBase *, SerializableBaseDictionary);

#endif // _SERIALIZABLEBASE_H