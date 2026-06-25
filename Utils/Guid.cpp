#include "stdwx.h"
#include "Guid.h"
#include <wx/string.h>
#if defined(__WXMSW__)
#include <wx/msw/ole/uuid.h>
#endif

wxString GenerateGuid()
{
#if defined(__WXMSW__)
	Uuid uid;
	uid.Create();
	return wxString::Format(wxT("{%s}"), (const wxChar*)uid).Upper();
#else
    srand(time(NULL));
    return wxString::Format(wxT("{%x%x-%x-%x-%x-%x%x%x}"),
    rand(), rand(),                 // Generates a 64-bit Hex number
    rand(),                         // Generates a 32-bit Hex number
    ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
    rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
    rand(), rand(), rand());        // Generates a 96-bit Hex number
#endif
}
