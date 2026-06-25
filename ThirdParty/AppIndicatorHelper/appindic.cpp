/* 
 * File:   appindic.cpp
 * Author: nmset@netcourrier.com (Work done as hobbyist)
 * License: LGPL version 2.1
 * Copyright: nmset@netcourrier.com
 * 
 * Created on 8 septembre 2015, 21:33
 */

#ifdef __WXGTK__

#include "appindic.h"

IMPLEMENT_DYNAMIC_CLASS(AppIndicatorHelper, wxObject)

AppIndicatorHelper::AppIndicatorHelper()
:wxObject()
{
    m_appMenu = NULL;
}

AppIndicatorHelper::AppIndicatorHelper(const wxString& id,
                               const wxString& iconFileName,
                               const wxString& iconDirPath,
                               AppIndicatorCategory category)
:wxObject()
{
    m_appMenu = NULL;
    printf("Icon: %s\n", iconFileName.utf8_str().data());
    m_indicator = app_indicator_new_with_path(id.utf8_str().data(),
                                              iconFileName.utf8_str().data(),
                                              category,
                                              iconDirPath.utf8_str().data());
}

AppIndicatorHelper::AppIndicatorHelper(const wxString& id, const wxString& iconFullFilePath, AppIndicatorCategory category)
:wxObject()
{
    m_appMenu = NULL;
    printf("Icon: %s\n", iconFullFilePath.utf8_str().data());
    m_indicator = app_indicator_new(id.utf8_str().data(),
                                              iconFullFilePath.utf8_str().data(),
                                              category);
    if(m_indicator != NULL)
    {
	printf("Indicator created.\n");
    }
}


AppIndicatorHelper::~AppIndicatorHelper()
{   
    g_object_unref((gpointer) m_indicator);
}

// Main wrapper functions.
void AppIndicatorHelper::SetTitle(const wxString& title)
{
    app_indicator_set_title(m_indicator, title.utf8_str().data());
}

const wxString AppIndicatorHelper::GetTitle() const
{
    return app_indicator_get_title(m_indicator);
}

void AppIndicatorHelper::SetStatus(AppIndicatorStatus status)
{
    app_indicator_set_status(m_indicator, status);
}

const AppIndicatorStatus AppIndicatorHelper::GetStatus() const
{
    return app_indicator_get_status(m_indicator);
}

// The main functionality of this class.
void AppIndicatorHelper::SetMenu(wxMenu * appMenu)
{
    wxASSERT_MSG(appMenu != NULL, _T("appMenu is NULL."));
    m_appMenu = appMenu;
    app_indicator_set_menu(m_indicator, GTK_MENU(m_appMenu->m_menu));
}

// A series of secondary wrapper functions.
const wxString AppIndicatorHelper::GetId() const
{
    return app_indicator_get_id(m_indicator);
}

const AppIndicatorCategory AppIndicatorHelper::GetCategory() const
{
    return app_indicator_get_category(m_indicator);
}

void AppIndicatorHelper::SetIconDir(const wxString& iconDir)
{
    app_indicator_set_icon_theme_path(m_indicator, iconDir.utf8_str().data());
}

const wxString AppIndicatorHelper::GetIconDir() const
{
    return app_indicator_get_icon_theme_path(m_indicator);
}

void AppIndicatorHelper::SetIcon(const wxString& iconFileName)
{
    app_indicator_set_icon(m_indicator, iconFileName.utf8_str().data());
}

const wxString AppIndicatorHelper::GetIcon() const
{
    return app_indicator_get_icon(m_indicator);
}

void AppIndicatorHelper::SetIconFull(const wxString& iconFileName, const wxString& iconDesc)
{
    app_indicator_set_icon_full(m_indicator, iconFileName.utf8_str().data(), iconDesc.utf8_str().data());
}

const wxString AppIndicatorHelper::GetIconDesc() const
{
    return app_indicator_get_icon_desc(m_indicator);
}

void AppIndicatorHelper::SetAttentionIcon(const wxString& iconFileName)
{
    app_indicator_set_attention_icon(m_indicator, iconFileName.utf8_str().data());
}

const wxString AppIndicatorHelper::GetAttentionIcon() const
{
    return app_indicator_get_attention_icon(m_indicator);
}

void AppIndicatorHelper::SetAttentionIconFull(const wxString& iconFileName, const wxString& iconDesc)
{
    app_indicator_set_attention_icon_full(m_indicator, iconFileName.utf8_str().data(), iconDesc.utf8_str().data());
}

const wxString AppIndicatorHelper::GetAttentionIconDesc() const
{
    return app_indicator_get_attention_icon_desc(m_indicator);
}

void AppIndicatorHelper::SetLabel(const wxString& label, const wxString& guide)
{
    app_indicator_set_label(m_indicator, label.utf8_str().data(), guide.utf8_str().data());
}

const wxString AppIndicatorHelper::GetLabel() const
{
    return app_indicator_get_label(m_indicator);
}

const wxString AppIndicatorHelper::GetLabelGuide() const
{
    return app_indicator_get_label_guide(m_indicator);
}

void AppIndicatorHelper::SetOrderingIndex(uint orderingIndex)
{
    app_indicator_set_ordering_index(m_indicator, orderingIndex);
}

const uint AppIndicatorHelper::GetOrderingIndex() const
{
    return app_indicator_get_ordering_index(m_indicator);
}

#endif
