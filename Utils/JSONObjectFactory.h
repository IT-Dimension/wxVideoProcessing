#pragma once 

class wxJSONValue;

#define JSON_KEY_ACTION wxT("action")
#define JSON_KEY_PROCESSED wxT("processed")

class JSONObjectFactory
{
public:
	static wxJSONValue Create(const wxString & action);
};