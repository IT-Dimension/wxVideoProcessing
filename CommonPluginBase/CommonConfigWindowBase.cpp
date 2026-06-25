#include "stdwx.h"
#include "CommonConfigWindowBase.h"

IMPLEMENT_DYNAMIC_CLASS(CommonConfigWindowBase, wxPanel)

CommonConfigWindowBase::CommonConfigWindowBase()
{

}

CommonConfigWindowBase::CommonConfigWindowBase(wxWindow *parent,
											   wxWindowID winid /*= wxID_ANY*/,
											   const wxPoint& pos /*= wxDefaultPosition*/,
											   const wxSize& size /*= wxDefaultSize*/,
											   long style /*= wxTAB_TRAVERSAL | wxNO_BORDER*/,
											   const wxString& name /*= wxPanelNameStr*/)
{
	Create(parent, winid, pos, size, style,	name);
}

bool CommonConfigWindowBase::Create(wxWindow *parent,
									wxWindowID winid /*= wxID_ANY*/,
									const wxPoint& pos /*= wxDefaultPosition*/,
									const wxSize& size /*= wxDefaultSize*/,
									long style /*= wxTAB_TRAVERSAL | wxNO_BORDER*/, 
									const wxString& name /*= wxPanelNameStr*/)
{
	return wxPanel::Create(parent, winid, pos, size, style, name);
}

CommonConfigWindowBase::~CommonConfigWindowBase(void)
{

}

bool CommonConfigWindowBase::ReadConfig()
{
	return true;
}

bool CommonConfigWindowBase::SaveConfig()
{
	return true;
}