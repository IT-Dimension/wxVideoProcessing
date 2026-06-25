/* 
 * File:   AIHTest.cpp
 * Author: nmset@netcourrier.com
 * License : irrelevant
 * 
 * Created on 11 septembre 2015, 16:58
 */

#include "AIHTest.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "stop.xpm"
#include <iostream>
using namespace std;

IMPLEMENT_APP(AIHTest)

AIHTest::AIHTest()
{
}

AIHTest::~AIHTest()
{
}

bool AIHTest::OnInit()
{
    AIHTestFrame * main = new AIHTestFrame(NULL, wxID_ANY, _T("AppIndicatorHelper test"));
    SetAppName(_T("AIHTest"));
    SetTopWindow(main);
    main->Show();
    return true;
}

int AIHTest::OnExit()
{
    return wxApp::OnExit();
}

AIHTestFrame::AIHTestFrame(wxWindow* parent, wxWindowID id, const wxString& title)
: wxFrame(parent, id, title)
{
    wxBoxSizer * szMain = new wxBoxSizer(wxVERTICAL);
    SetSizer(szMain);
    m_itemID = -10000;
    m_subID = 0;
    m_mainMenu = NULL;
    m_removedMenu = NULL;
    SetToolTip(_T("Right click : Popup menu.\nDouble click : Delete menu."));
    m_mainMenu = CreateMenu();
    Bind(wxEVT_RIGHT_UP, &AIHTestFrame::ShowMenu, this);
    Bind(wxEVT_LEFT_DCLICK, &AIHTestFrame::DeleteMenu, this);
    const wxFileName exe(wxStandardPaths::Get().GetExecutablePath());
    m_indicator = new AppIndicatorHelper(_T("AIH"),
                                         exe.GetPath(wxPATH_GET_SEPARATOR) + _T("kde.png"),
                                         APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    m_indicator->SetTitle((_T("AppIndicator helper test")));
    m_indicator->SetMenu(m_mainMenu);
    m_indicator->SetStatus(APP_INDICATOR_STATUS_ACTIVE);
}

AIHTestFrame::~AIHTestFrame()
{
    delete m_mainMenu;
    delete m_indicator;
}

wxMenu* AIHTestFrame::CreateMenu()
{
    wxMenu * menu = new wxMenu();
    wxString label;
    label = _T("SUB ") + wxVariant(m_subID).GetString();
    if (m_mainMenu != NULL) menu->SetTitle(label);
    label = _T("New ")  + wxVariant(m_itemID).GetString();
    menu->Append(m_itemID, label);
    m_itemID++;
    label = _T("Delete ")  + wxVariant(m_itemID).GetString();
    wxMenuItem * item = new wxMenuItem(menu, m_itemID, label);
    item->SetBitmap(stop_xpm);
    menu->Append(item);
    m_itemID++;
    menu->AppendSeparator();
    
    label = _T("RadioA ")  + wxVariant(m_itemID).GetString();
    menu->AppendRadioItem(m_itemID, label);
    m_itemID++;
    label = _T("RadioB ")  + wxVariant(m_itemID).GetString();
    menu->AppendRadioItem(m_itemID, label);
    m_itemID++;
    menu->AppendSeparator();
    
    label = _T("Check ")  + wxVariant(m_itemID).GetString();
    menu->AppendCheckItem(m_itemID, label);
    m_itemID++;
    menu->AppendSeparator();
    
    label = _T("RadioC ")  + wxVariant(m_itemID).GetString();
    menu->AppendRadioItem(m_itemID, label);
    m_itemID++;
    label = _T("RadioD ")  + wxVariant(m_itemID).GetString();
    menu->AppendRadioItem(m_itemID, label);
    m_itemID++;
    menu->AppendSeparator();
    
    menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &AIHTestFrame::OnMenuItemClick, this);
    m_subID++;
    return menu;
}

void AIHTestFrame::ShowMenu(wxMouseEvent& evt)
{
    if (m_mainMenu != NULL) PopupMenu(m_mainMenu);
}

void AIHTestFrame::DeleteMenu(wxMouseEvent& evt)
{
    wxDELETE(m_mainMenu);
    // If we destroy the wxWidgets reference menu, we must get rid of
    // the indicator, to avoid an inevitable crash on item select.
    wxDELETE(m_indicator);
}

/*
 * The 'New' and 'Delete' actions are just to show the need of calling SetMenu()
 * again. This should be done whenever items are removed, added or modified.
 * If check or radio items are clicked in the wxWidgets application, we need not
 * call SetMenu(), this is done by AppIndicatorHelper.
 */
void AIHTestFrame::OnMenuItemClick(wxCommandEvent& evt)
{
    wxMenu * menu = static_cast<wxMenu*> (evt.GetEventObject());
    wxMenuItem * item = menu->FindItem(evt.GetId());
    const wxString label = item->GetItemLabelText();
    if (label.StartsWith(_T("New "))) {
        wxMenu * subMenu = CreateMenu();
        wxMenuItem * subItem = menu->AppendSubMenu(subMenu, subMenu->GetTitle());
        subMenu->SetClientData(subItem);
        m_indicator->SetMenu(m_mainMenu); // <-
    }
    if (label.StartsWith(_T("Delete "))) {
        if (menu != m_mainMenu) {
            wxMenuItem * subItem = static_cast<wxMenuItem*> (menu->GetClientData());
            // Can't destroy the submenu when it is shown.
            subItem->GetMenu()->Remove(subItem);
            m_indicator->SetMenu(m_mainMenu); // <-
            // We resort to a trick by deleting the removed menu in idle time.
            m_removedMenu = menu;
            Bind(wxEVT_IDLE, &AIHTestFrame::OnIdle, this);
        }
    }
    evt.Skip();
}

void AIHTestFrame::OnIdle(wxIdleEvent& evt)
{
    wxDELETE(m_removedMenu);
    Unbind(wxEVT_IDLE, &AIHTestFrame::OnIdle, this);
}
