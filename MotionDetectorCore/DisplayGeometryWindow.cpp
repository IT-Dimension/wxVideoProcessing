#include "stdwx.h"
#include "DisplayGeometryWindow.h"
#include <wx/dcbuffer.h>

#if defined(USE_VLD)
#include <vld.h>
#endif

IMPLEMENT_DYNAMIC_CLASS(DisplayGeometryWindow, wxWindow)

BEGIN_EVENT_TABLE(DisplayGeometryWindow, wxWindow)
	EVT_SIZE(DisplayGeometryWindow::OnSize)
	EVT_PAINT(DisplayGeometryWindow::OnPaint)
	EVT_ERASE_BACKGROUND(DisplayGeometryWindow::OnEraseBackground)
END_EVENT_TABLE()

DisplayGeometryWindow::DisplayGeometryWindow()
	: m_GeometryProvider(GeometryProvider(nullptr, nullptr, nullptr))
{
	Init();
}

DisplayGeometryWindow::DisplayGeometryWindow(wxWindow* parent, const GeometryProvider & geometryProvider, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
	: m_GeometryProvider(geometryProvider)
{
	Init();
	Create(parent, id, pos, size, style);
}

bool DisplayGeometryWindow::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	wxWindow::Create(parent, id, pos, size, style);
	return true;
}

DisplayGeometryWindow::~DisplayGeometryWindow()
{
}

void DisplayGeometryWindow::Init()
{
	m_Scale = 1;
	m_Border = 1;
}

void DisplayGeometryWindow::OnEraseBackground(wxEraseEvent& event)
{
}

void DisplayGeometryWindow::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	dc.SetBackground(wxBrush(GetBackgroundColour()));
	dc.Clear();

	if (m_Bitmap.IsOk())
		dc.DrawBitmap(m_Bitmap, m_DrawRect.GetTopLeft());
}

void DisplayGeometryWindow::OnSize(wxSizeEvent& event)
{
	CalcDrawRect();
}

void DisplayGeometryWindow::CalcDrawRect()
{
	wxRectVector displays;
	m_GeometryProvider.GetGeometry(displays);
	m_DisplayRect = GetDisplayRect(displays);

	wxSize clientSize = GetClientSize();
	wxSize bestSize = clientSize - wxSize(m_Border * 2, m_Border * 2); // Add border
	wxSize bitmapSize = m_DisplayRect.GetSize();
	double kx = (double) bestSize.x / bitmapSize.x;
	double ky = (double) bestSize.y / bitmapSize.y;
	if (kx < ky)
	{
		m_DrawRect.width = bestSize.x;
		m_DrawRect.height = (kx * bitmapSize.y);
		m_Scale = kx;
	}
	else
	{
		m_DrawRect.height = bestSize.y;
		m_DrawRect.width = (ky * bitmapSize.x);
		m_Scale = ky;
	}
	m_DrawRect = m_DrawRect.CenterIn(wxRect(clientSize));
	m_DrawRect.x = m_Border;

	CreateBitmap();

	Refresh();
}

wxRect DisplayGeometryWindow::GetDisplayRect(const wxRectVector & displays)
{
	wxRect resolution;
	for (auto i = 0; i < displays.size(); ++i)
		resolution.Union(displays[i]);
	return resolution;
}

void DisplayGeometryWindow::CreateBitmap()
{
	m_Bitmap = wxBitmap(m_DrawRect.GetSize() + wxSize(1, 1));
	wxMemoryDC mdc(m_Bitmap);
	mdc.SetBackground(wxBrush(GetBackgroundColour()));
	mdc.Clear();
	mdc.SetBrush(*wxTRANSPARENT_BRUSH);
	mdc.SetPen(*wxBLACK_PEN);
	mdc.SetTextForeground(*wxBLACK);

	wxRectVector displays;
	m_GeometryProvider.GetGeometry(displays);
	wxRect resolution = GetDisplayRect(displays);

	for (auto i = 0; i < displays.size(); ++i)
	{
		wxRect rect = displays[i];
		rect.Offset(-resolution.GetTopLeft());
		wxPoint p1 = rect.GetTopLeft();
		wxPoint p2 = rect.GetBottomRight();
		p1.x *= m_Scale;
		p1.y *= m_Scale;
		p2.x *= m_Scale;
		p2.y *= m_Scale;
		rect = wxRect(p1, p2);

		mdc.DrawRectangle(rect);

		mdc.DrawLabel(wxString::Format(wxT("#%d"), i + 1), rect, wxALIGN_CENTER);
	}

}