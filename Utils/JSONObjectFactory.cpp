#include "stdwx.h"
#include <wx/jsonval.h>
#include "JSONObjectFactory.h"

wxJSONValue JSONObjectFactory::Create(const wxString & action)
{
	wxJSONValue result;
	result[JSON_KEY_ACTION] = action;
	return result;
}