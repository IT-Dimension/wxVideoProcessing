#ifndef _DISPLAYGEOMETRYWINDOW_H
#define _DISPLAYGEOMETRYWINDOW_H

#include "MotionDetectorCorePlugin.h"
#include <GeometryProvider.h>

class MOTION_DETECTOR_CORE_API DisplayGeometryWindow: public wxWindow
{
	DECLARE_DYNAMIC_CLASS(DisplayGeometryWindow)
	DECLARE_EVENT_TABLE()
public:
	DisplayGeometryWindow();
	DisplayGeometryWindow(wxWindow * parent, const GeometryProvider & geometryPrivoder, wxWindowID id = wxID_STATIC, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSIMPLE_BORDER);

	bool Create(wxWindow * parent, wxWindowID id = wxID_STATIC, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSIMPLE_BORDER);

	~DisplayGeometryWindow();

	void Init();

protected:
	void OnSize(wxSizeEvent & event);
	void OnPaint(wxPaintEvent & event);
	void OnEraseBackground(wxEraseEvent & event);

	void CalcDrawRect();
	static wxRect GetDisplayRect(const wxRectVector & displays);
	void CreateBitmap();

protected:
	const GeometryProvider & m_GeometryProvider;
	wxRect m_DrawRect;
	wxBitmap m_Bitmap;
	wxRect m_DisplayRect;
	double m_Scale;
	int m_Border;
};

#endif // _DISPLAYGEOMETRYWINDOW_H
