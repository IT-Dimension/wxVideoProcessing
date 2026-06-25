/* 
 * File:   appindic.h
 * Author: nmset@netcourrier.com (Work done as hobbyist)
 * License: LGPL version 2.1
 * Copyright: nmset@netcourrier.com
 *
 * Created on 8 septembre 2015, 21:33
 */

#ifdef __WXGTK__
#ifndef APPINDICATORHELPER_H
#define	APPINDICATORHELPER_H

#include <wx/wx.h>
#include <libappindicator/app-indicator.h>

/**
 * This class is a helper for the libappindicator library. It's aim is to 
 * facilitate 'put an icon' in the system tray with wxWidgets, while hiding
 * all C/GTK code. It is intended for WXGTK builds only.
 * 
 * An application must pass a wxMenu object to an instance of
 * this class, and it will show the GTK menu member of the wxMenu with a right
 * click on the system tray icon. The passed wxMenu may or may not be accessed
 * in the wxWidgets application window.
 * 
 * The notion of a 'GTK icon theme' is downplayed. An icon is passed to this
 * class with its complete file name and the directory path separately.
 * Alternatively, a constructor is provided for a full path to the icon.
 * 
 * The following libappindicator functions are simply ignored :
 * app_indicator_set_secondary_activate_target,
 * app_indicator_get_secondary_activate_target,
 * app_indicator_build_menu_from_desktop.
 * 
 * Note : If an application must delete the wxWidgets reference menu, it must
 * delete the AppIndicatorHelper instance.
 * 
 * This project is inspired from
 * https://github.com/jarod/libappindicator-qt
 * 
 * @return 
 */

class AppIndicatorHelper : public wxObject {
    DECLARE_DYNAMIC_CLASS(AppIndicatorHelper);
public:
    AppIndicatorHelper();
    /**
     * 
     * @param id A unique identifier.
     * @param iconFileName The filename of an icon, with its extension.
     * @param iconDirPath The full path to the icon
     * @param category The category of the icon, listed in AppIndicatorCategory
     * enum.
     */
    AppIndicatorHelper(const wxString& id,
            const wxString& iconFileName,
            const wxString& iconDirPath,
            AppIndicatorCategory category);
    /**
     * 
     * @param id A unique identifier.
     * @param iconFullFilePath The absolute path of an icon, with its extension.
     * @param iconDirPath The full path to the icon
     * @param category The category of the icon, listed in AppIndicatorCategory
     * enum.
     */
    AppIndicatorHelper(const wxString& id,
            const wxString& iconFullFilePath,
            AppIndicatorCategory category);
    virtual ~AppIndicatorHelper();

    void SetTitle(const wxString& title);
    const wxString GetTitle() const;
    void SetStatus(AppIndicatorStatus status);
    const AppIndicatorStatus GetStatus() const;
    /**
     * 
     * @param appMenu A wxMenu managed in your application, which may or may not
     * show this menu.
     * 
     * No icon will appear in the system tray without a menu.
     */
    void SetMenu(wxMenu * appMenu);

    const wxString GetId() const;
    const AppIndicatorCategory GetCategory() const;
    void SetIconDir(const wxString& iconDir);
    const wxString GetIconDir() const;
    void SetIcon(const wxString& iconFileName);
    const wxString GetIcon() const;
    /**
     * 
     * @param iconFileName
     * @param iconDesc From app-indicator.c
     * The description of the regular icon that is shown for the indicator.
     */
    void SetIconFull(const wxString& iconFileName, const wxString& iconDesc);
    const wxString GetIconDesc() const;
    /**
     * 
     * @param iconFileName The file name with extension of an icon to use when
     * the status is set to APP_INDICATOR_STATUS_ATTENTION. The icon must be in
     * the directory given by GetIconDir().
     */
    void SetAttentionIcon(const wxString& iconFileName);
    const wxString GetAttentionIcon() const;
    void SetAttentionIconFull(const wxString& iconFileName, const wxString& iconDesc);
    const wxString GetAttentionIconDesc() const;
    /**
     * 
     * @param label From app-indicator.c
     * A label that can be shown next to the string in the application
     * indicator.  The label will not be shown unless there is an icon
     * as well.  The label is useful for numerical and other frequently
     * updated information.  In general, it shouldn't be shown unless a
     * user requests it as it can take up a significant amount of space
     * on the user's panel.  This may not be shown in all visualizations.
     * @param guide From app-indicator.c
     * An optional string to provide guidance to the panel on how big
     * the #AppIndicator:label string could get.  If this is set correctly
     * then the panel should never 'jiggle' as the string adjusts through
     * out the range of options.  For instance, if you were providing a
     * percentage like "54% thrust" in #AppIndicator:label you'd want to
     * set this string to "100% thrust" to ensure space when Scotty can
     * get you enough power.
     */
    void SetLabel(const wxString& label, const wxString& guide);
    const wxString GetLabel() const;
    const wxString GetLabelGuide()const;
    /**
     * 
     * @param orderingIndex From app-indicator.c 
     * The ordering index is an odd parameter, and if you think you don't need
     * it you're probably right.  In general, the application indicator try
     * to place the applications in a recreatable place taking into account
     * which category they're in to try and group them.  But, there are some
     * cases where you'd want to ensure indicators are next to each other.
     * To do that you can override the generated ordering index and replace it
     * with a new one.  Again, you probably don't want to be doing this, but
     * in case you do, this is the way.
     */
    void SetOrderingIndex(uint orderingIndex);
    const uint GetOrderingIndex() const;
private:
    AppIndicator * m_indicator;
    //The application's wxMenu.
    wxWeakRef<wxMenu> m_appMenu;
    //m_appMenu.SetIndicator(m_indicator);

};

#endif	/* APPINDICATORHELPER_H */
#endif

