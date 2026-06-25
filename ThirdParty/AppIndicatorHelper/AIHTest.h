/* 
 * File:   AIHTest.h
 * Author: nmset@netcourrier.com
 * License : irrelevant
 *
 * Created on 11 septembre 2015, 16:57
 */

#ifndef AIHTEST_H
#define	AIHTEST_H

#include <wx/wx.h>
#include "appindic.h"

class AIHTest : public wxApp {
public:
    AIHTest();
    virtual ~AIHTest();

    bool OnInit();
    int OnExit();
private:

};

class AIHTestFrame : public wxFrame {
public:
    AIHTestFrame(wxWindow * parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString);
    virtual ~AIHTestFrame();
private:
    wxMenu * m_mainMenu;
    wxMenu * m_removedMenu;
    long m_itemID;
    long m_subID;
    AppIndicatorHelper * m_indicator;
    
    wxMenu * CreateMenu();
    // The menus is shown here for the purpose of the test.
    // A real application needs not show this menu at all.
    void ShowMenu(wxMouseEvent& evt);
    void DeleteMenu(wxMouseEvent& evt);
    void OnMenuItemClick(wxCommandEvent& evt);
    void OnIdle(wxIdleEvent& evt);
};

#endif	/* AIHTEST_H */

