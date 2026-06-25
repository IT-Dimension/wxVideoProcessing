#ifndef _COMMONCONFIGWINDOWBASE_H
#define _COMMONCONFIGWINDOWBASE_H
#include "CommonPlugin.h"
#include <wx/panel.h>

class IFLOOR_API_COMMONPLUGINBASE CommonConfigWindowBase : public wxPanel 
{
	DECLARE_DYNAMIC_CLASS(CommonConfigWindowBase)
public:
	/// Constructors
	CommonConfigWindowBase();
	CommonConfigWindowBase(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	/// Pseudo ctor
	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	virtual ~CommonConfigWindowBase();

	/// Reads config from the effect
	virtual bool ReadConfig();

	/// Saves config to the effect
	virtual bool SaveConfig();
};


#endif // _COMMONCONFIGWINDOWBASE_H
