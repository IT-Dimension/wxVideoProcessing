#include "stdwx.h"
#include "FilterPreviewWindow.h"
#include <wx/dcbuffer.h>

IMPLEMENT_DYNAMIC_CLASS(FilterPreviewWindow, wxWindow)

BEGIN_EVENT_TABLE(FilterPreviewWindow, wxWindow)
	EVT_SIZE(FilterPreviewWindow::OnSize)
	EVT_PAINT(FilterPreviewWindow::OnPaint)
	EVT_ERASE_BACKGROUND(FilterPreviewWindow::OnEraseBackground)
END_EVENT_TABLE()

FilterPreviewWindow::FilterPreviewWindow()
{
	Init();
}

FilterPreviewWindow::FilterPreviewWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	Init();
	Create(parent, id, pos, size, style);
}

bool FilterPreviewWindow::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	wxWindow::Create(parent, id, pos, size, style);
	return true;
}

FilterPreviewWindow::~FilterPreviewWindow()
{
}

void FilterPreviewWindow::Init()
{
}

void FilterPreviewWindow::OnEraseBackground(wxEraseEvent& event)
{
}

void FilterPreviewWindow::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	dc.SetBackground(wxBrush(GetBackgroundColour()));
	dc.Clear();

	if (m_Bitmap.IsOk())
	{
		wxMemoryDC mdc(m_Bitmap);
		dc.StretchBlit(m_DrawRect.GetTopLeft(), m_DrawRect.GetSize(), &mdc, wxPoint(0, 0), m_Bitmap.GetSize());
	}
	else // No Image
	{
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(m_DrawRect);
		dc.SetTextForeground(*wxWHITE);
		dc.DrawLabel(_("No Image"), m_DrawRect, wxALIGN_CENTER);
	}
}

void FilterPreviewWindow::OnSize(wxSizeEvent& event)
{
	CalcDrawRect();
}

void FilterPreviewWindow::SetBitmap(const wxBitmap & bitmap)
{
	m_Bitmap = bitmap;
	CalcDrawRect();
	Refresh();
}

void FilterPreviewWindow::CalcDrawRect()
{
	wxSize size = GetClientSize();
	if (m_Bitmap.IsOk())
	{
		wxSize bitmapSize = m_Bitmap.GetSize();
		double kx = (double) size.x / bitmapSize.x;
		double ky = (double) size.y / bitmapSize.y;
		if (kx < ky)
		{
			m_DrawRect.width = size.x;
			m_DrawRect.height = (kx * bitmapSize.y);
		}
		else
		{
			m_DrawRect.height = size.y;
			m_DrawRect.width = (ky * bitmapSize.x);
		}
		m_DrawRect = m_DrawRect.CenterIn(wxRect(size));
	}
	else
	{
		m_DrawRect = wxRect(size);
	}
	Refresh();
}
