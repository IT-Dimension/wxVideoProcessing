#ifndef _EFFECTPREVIEWWINDOW_H
#define _EFFECTPREVIEWWINDOW_H

#include "MotionDetectorCorePlugin.h"

class MOTION_DETECTOR_CORE_API FilterPreviewWindow: public wxWindow
{
	DECLARE_DYNAMIC_CLASS(FilterPreviewWindow)
	DECLARE_EVENT_TABLE()
public:
	FilterPreviewWindow();
	FilterPreviewWindow(wxWindow * parent, wxWindowID id = wxID_STATIC, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSIMPLE_BORDER);

	bool Create(wxWindow * parent, wxWindowID id = wxID_STATIC, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSIMPLE_BORDER);

	~FilterPreviewWindow();

	void Init();

	void SetBitmap(const wxBitmap & bitmap);

protected:
	void OnSize(wxSizeEvent & event);
	void OnPaint(wxPaintEvent & event);
	void OnEraseBackground(wxEraseEvent & event);

	void CalcDrawRect();

protected:
	wxRect m_DrawRect;
	wxBitmap m_Bitmap;
};

#endif // _EFFECTPREVIEWWINDOW_H
