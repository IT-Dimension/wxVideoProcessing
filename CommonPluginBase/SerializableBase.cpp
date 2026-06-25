#include "stdwx.h"
#include "SerializableBase.h"
#include <wx/listimpl.cpp>

IMPLEMENT_ABSTRACT_CLASS(SerializableBase, xsSerializable);

WX_DEFINE_USER_EXPORTED_LIST(SerializableBaseList);

SerializableBase::SerializableBase()
{
}

SerializableBase::~SerializableBase()
{
}

bool SerializableBase::Deserialize(wxInputStream & instream)
{
	return Deserialize(instream, *this);
}

bool SerializableBase::Deserialize(wxInputStream & instream, xsSerializable & obj)
{
	wxXmlSerializer Serializer;
	Serializer.EnableCloning(false);
	Serializer.SetRootItem(&obj);
	bool res = Serializer.DeserializeFromXml(instream);
	Serializer.SetRootItem(nullptr, false);
	obj.Reparent(nullptr);
	obj.SetParentManager(nullptr);
	return res;
}

bool SerializableBase::Deserialize(const wxString & config, xsSerializable & obj)
{
	wxStringInputStream stream(config);
	return Deserialize(stream, obj);
}

bool SerializableBase::Serialize(wxOutputStream & outstream)
{
	return Serialize(outstream, *this);
}

bool SerializableBase::Serialize(wxOutputStream & outstream, xsSerializable & obj)
{
	wxXmlSerializer Serializer;
	Serializer.EnableCloning(false);
	Serializer.SetRootItem(&obj);
	bool res = Serializer.SerializeToXml(outstream, true);
	Serializer.SetRootItem(nullptr, false);
	obj.Reparent(nullptr);
	obj.SetParentManager(nullptr);
	return res;
}

wxString SerializableBase::Serialize(xsSerializable & obj)
{
	wxString config;
	wxStringOutputStream stream(&config);
	Serialize(stream, obj);
	return config;
}