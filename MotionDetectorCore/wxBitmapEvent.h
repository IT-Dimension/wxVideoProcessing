#ifndef _WXBITMAPEVENT_H
#define _WXBITMAPEVENT_H

class wxBitmapEvent: public wxCommandEvent
{
public:
	wxBitmapEvent(wxBitmap * bitmap = NULL, wxEventType commandType = wxEVT_NULL, int winid = 0)
		: wxCommandEvent(commandType, winid)
		, m_Bitmap(bitmap)
	{
	}

	wxBitmapEvent(const wxBitmapEvent & event)
		: wxCommandEvent(event)
	{
		if (event.m_Bitmap)
			m_Bitmap = new wxBitmap(*event.m_Bitmap);
		else
			m_Bitmap = NULL;
	}

	~wxBitmapEvent()
	{
		wxDELETE(m_Bitmap);
	}

	void SetBitmap(wxBitmap * val) { m_Bitmap = val; }
	wxBitmap * GetBitmap() const { return m_Bitmap; }
	wxBitmap * DetachBitmap()
	{ 
		wxBitmap * tmp = m_Bitmap;
		m_Bitmap = NULL;
		return tmp; 
	}

	// implement the base class pure virtual
	virtual wxEvent *Clone() const { return new wxBitmapEvent(*this); }

private:
	wxBitmap * m_Bitmap;
};


wxDEFINE_EVENT(EVT_IMAGE_CALLBACK, wxBitmapEvent);

#define wxBitmapEventHandler(func) (&func)

#define EVT_BITMAP_SEND(id, func) \
	wx__DECLARE_EVT1(EVT_IMAGE_CALLBACK, id, wxBitmapEventHandler(func))


#endif // _WXBITMAPEVENT_H