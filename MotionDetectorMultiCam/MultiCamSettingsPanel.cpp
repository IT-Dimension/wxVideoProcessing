#include "stdwx.h"
#include "MultiCamSettingsPanel.h"
#include "MotionDetectorMultiCam.h"
#include "IFloorImagePreprocessor.h"
#include "FilterPreviewWindow.h"
#include <wx/listctrl.h>
#include <wxTemplateClientData.h>
#include <wxBitmapEvent.h>
#include <wx/valgen.h>

////@begin includes
#include "DisplayGeometryWindow.h"
#include "wx/imaglist.h"
////@end includes

#if defined(USE_VLD)
#include <vld.h>
#endif

////@begin XPM images
////@end XPM images

/*
 * MultiCamSettingsPanel type definition
 */

IMPLEMENT_DYNAMIC_CLASS( MultiCamSettingsPanel, MotionDetectorConfigWindowBase )


/*
 * MultiCamSettingsPanel event table definition
 */

BEGIN_EVENT_TABLE( MultiCamSettingsPanel, MotionDetectorConfigWindowBase )

////@begin MultiCamSettingsPanel event table entries
////@end MultiCamSettingsPanel event table entries
EVT_BITMAP_SEND(wxID_ANY, MultiCamSettingsPanel::OnImageCallback)

END_EVENT_TABLE()


/*
 * MultiCamSettingsPanel constructors
 */

 MultiCamSettingsPanel::MultiCamSettingsPanel()
{
	Init();
}

MultiCamSettingsPanel::MultiCamSettingsPanel(MotionDetectorMultiCam * detector, wxWindow * parent)
{
	Init();
	Create(detector, parent);
}


/*
 * MultiCamSettingsPanel creator
 */

bool MultiCamSettingsPanel::Create(MotionDetectorMultiCam * detector, wxWindow * parent)
{
	MotionDetectorConfigWindowBase::Create(detector, parent);
	CreateControls();
	ReadConfig();
	return true;
}


/*
 * MultiCamSettingsPanel destructor
 */

MultiCamSettingsPanel::~MultiCamSettingsPanel()
{
////@begin MultiCamSettingsPanel destruction
////@end MultiCamSettingsPanel destruction
	m_CameraSettings.Clear();
}


/*
 * Member initialisation
 */

void MultiCamSettingsPanel::Init()
{
////@begin MultiCamSettingsPanel member initialisation
	m_Notebook = NULL;
////@end MultiCamSettingsPanel member initialisation
}


/*
 * Control creation for MultiCamSettingsPanel
 */

void MultiCamSettingsPanel::CreateControls()
{
////@begin MultiCamSettingsPanel content construction
	MultiCamSettingsPanel* itemMotionDetectorConfigWindowBase1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemMotionDetectorConfigWindowBase1->SetSizer(itemBoxSizer2);

	wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemMotionDetectorConfigWindowBase1, wxID_ANY, _("Display Settings"));
	wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxHORIZONTAL);
	itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer3->GetStaticBox(), wxID_STATIC, _("Displays layout:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticBoxSizer3->Add(itemStaticText4, 0, wxALIGN_TOP|wxALL, 5);

	DisplayGeometryWindow* itemWindow5 = new DisplayGeometryWindow( itemStaticBoxSizer3->GetStaticBox(), m_Detector->GetGeometryProvider(), ID_DisplayGeometry, wxDefaultPosition, wxSize(120, 80), wxNO_BORDER );
	itemStaticBoxSizer3->Add(itemWindow5, 1, wxGROW|wxALL, 5);

	m_Notebook = new wxListbook( itemMotionDetectorConfigWindowBase1, ID_LISTBOOK, wxDefaultPosition, wxDefaultSize, wxBK_TOP );

	itemBoxSizer2->Add(m_Notebook, 1, wxGROW|wxALL, 5);

////@end MultiCamSettingsPanel content construction
	m_Notebook->GetListView()->SetSizeHints(-1, 50, -1, 50);

	wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemMotionDetectorConfigWindowBase1, wxID_ANY, _("Blobs Limits"));
	wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxHORIZONTAL);
	itemBoxSizer2->Add(itemStaticBoxSizer6, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticText* itemStaticText7 = new wxStaticText( itemMotionDetectorConfigWindowBase1, wxID_STATIC, _("Maximum Blob Area Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticBoxSizer6->Add(itemStaticText7, 0, wxALIGN_TOP|wxALL, 5);

	m_cbMaxBlobArea = new wxSpinCtrl( itemMotionDetectorConfigWindowBase1, ID_MAXBLOBAREA, _T("10000"), wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 1000, 1000000, 0 );
	itemStaticBoxSizer6->Add(m_cbMaxBlobArea, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticText* itemStaticText8 = new wxStaticText( itemMotionDetectorConfigWindowBase1, wxID_STATIC, _("Minimum Blob Area Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticBoxSizer6->Add(itemStaticText8, 0, wxALIGN_TOP|wxALL, 5);

	m_cbMinBlobArea = new wxSpinCtrl( itemMotionDetectorConfigWindowBase1, ID_MINBLOBAREA, _T("100"), wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 10, 10000, 0 );
	itemStaticBoxSizer6->Add(m_cbMinBlobArea, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticText* itemStaticText9 = new wxStaticText( itemMotionDetectorConfigWindowBase1, wxID_STATIC, _("Maximum Blob Count:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticBoxSizer6->Add(itemStaticText9, 0, wxALIGN_TOP|wxALL, 5);

	m_cbMaxBlobCount = new wxSpinCtrl( itemMotionDetectorConfigWindowBase1, ID_MAXBLOBCOUNT, _T("5"), wxDefaultPosition, wxSize(70, -1), wxSP_ARROW_KEYS, 1, 200, 0 );
	itemStaticBoxSizer6->Add(m_cbMaxBlobCount, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_cbUseOneOnMulti = new wxCheckBox( itemMotionDetectorConfigWindowBase1, ID_MAXBLOBCOUNT, _T("Use one sensor on multiple monitors"));
	itemStaticBoxSizer6->Add(m_cbUseOneOnMulti , 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	if(m_Detector != NULL){
		MotionDetectorMultiCam* detector = dynamic_cast<MotionDetectorMultiCam*>(m_Detector);
		m_cbMaxBlobArea->SetValidator(wxGenericValidator(&detector->m_maxBlobArea));
		m_cbMaxBlobArea->GetValidator()->TransferToWindow();

		m_cbMinBlobArea->SetValidator(wxGenericValidator(&detector->m_minBlobArea));
		m_cbMinBlobArea->GetValidator()->TransferToWindow();

		m_cbMaxBlobCount->SetValidator(wxGenericValidator(&detector->m_maxBlobCount));
		m_cbMaxBlobCount->GetValidator()->TransferToWindow();

		m_cbUseOneOnMulti->SetValidator(wxGenericValidator(&detector->m_bUseOneOnMulti));
		m_cbUseOneOnMulti->GetValidator()->TransferToWindow();
	}
#if defined(__WXGTK__)
    SetBackgroundColour(*wxWHITE);
#endif
}


/*
 * Should we show tooltips?
 */

bool MultiCamSettingsPanel::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap MultiCamSettingsPanel::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin MultiCamSettingsPanel bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end MultiCamSettingsPanel bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon MultiCamSettingsPanel::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin MultiCamSettingsPanel icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end MultiCamSettingsPanel icon retrieval
}

bool MultiCamSettingsPanel::ReadConfig()
{
	if (m_Detector == nullptr)
	{
		return false;
	}

	MotionDetectorMultiCam * detector = (MotionDetectorMultiCam *)m_Detector;
	m_CameraSettings = detector->GetCameraSettings();
	return true;
}

bool MultiCamSettingsPanel::SaveConfig()
{

	if (m_Detector == nullptr)
	{
		return false;
	}

	MotionDetectorMultiCam * detector = (MotionDetectorMultiCam *)m_Detector;
	ReadCameraSettings();
	detector->GetCameraSettings() = m_CameraSettings;
	return true;
}

void MultiCamSettingsPanel::StartDetection()
{
	wxWindowUpdateLocker lock(this);

	int pos = m_Notebook->GetSelection();
	m_Notebook->DeleteAllPages();
	m_PreviewMap.clear();
	for (size_t i = 0; i < m_Detector->GetSupportedCamerasCount(); ++i)
	{
		CreateCameraSettings(i);
	}

	TransferDataToWindow();

	if (pos == wxNOT_FOUND)
		pos = 0;
	if (m_Notebook->GetPageCount() > 0)
		m_Notebook->SetSelection(pos);
}

void MultiCamSettingsPanel::StopDetection()
{
	ReadCameraSettings();
}

void MultiCamSettingsPanel::CreateCameraSettings(int index)
{
	MotionDetectorMultiCam * detector = (MotionDetectorMultiCam *)m_Detector;
	IFloorImagePreprocessor * camera = detector->GetCameraImagePreprocessor(index);

	if (!camera)
		return;

	bool hasConfig = index < (int)m_CameraSettings.GetCount();

	wxScrolledWindow * page = new wxScrolledWindow(m_Notebook);
#if defined(__WXGTK__)
    page->SetBackgroundColour(*wxWHITE);
#endif
	page->SetClientData(camera);
	wxSizer * sizer = new wxFlexGridSizer(2);
	page->SetSizer(sizer);
	ChainOfFilters & filters = camera->GetFilters();
	for (size_t i = 0; i < filters.size(); ++i)
	{
		Filter * filter = filters[i];
		if (hasConfig)
			IFloorImagePreprocessor::SetFilterSettings(filter, m_CameraSettings[index][filter->GetClassInfo()->GetClassName()]);
		FilterPreviewWindow * preview = new FilterPreviewWindow(page, wxID_STATIC, wxDefaultPosition, wxSize(PREVIEW_WIDTH, PREVIEW_HEIGHT), wxNO_BORDER);
		m_PreviewMap[filter] = preview;
		filter->CreatePreviewAndParams(page, sizer, preview);
		filter->SetImageCallback(&MultiCamSettingsPanel::ImageCallback, index, this);
	}
	page->Layout();
	page->SetScrollbars(1, 1, 0, 0);
	page->FitInside();

	m_Notebook->AddPage(page, wxString::Format(_("Camera #%d"), index));
}

void MultiCamSettingsPanel::ImageCallback(int cameraID, const wxString& filterName, cv::Mat image, void* userData)
{
	MultiCamSettingsPanel * _this = (MultiCamSettingsPanel *)userData;
	int pos = _this->m_Notebook->GetSelection();
	if (pos != cameraID || image.empty())
		return;

	cv::Mat resizedImg;
	cv::resize(image, resizedImg, cv::Size(PREVIEW_WIDTH, PREVIEW_HEIGHT), 0, 0, cv::INTER_LINEAR);

	cv::Mat destination;
	if (resizedImg.channels() == 1)
	{
		cv::cvtColor(resizedImg, destination, cv::COLOR_GRAY2RGB);
	}
	else if (resizedImg.channels() == 3)
	{
		cv::cvtColor(resizedImg, destination, cv::COLOR_BGR2RGB);
	}
	else if (resizedImg.channels() == 4)
	{
		cv::cvtColor(resizedImg, destination, cv::COLOR_BGRA2RGB);
	}
	else
		wxFAIL_MSG(wxT("Unknown color model"));
	

	if (!destination.empty())
	{
		wxImage img(destination.cols, destination.rows, destination.data, true);
		wxBitmap* bmp = new wxBitmap(img);

		wxBitmapEvent* event = new wxBitmapEvent(bmp, EVT_IMAGE_CALLBACK);
		event->SetString(filterName);
		event->SetInt(cameraID);
		_this->GetEventHandler()->QueueEvent(event);
	}
}

void MultiCamSettingsPanel::OnImageCallback(wxBitmapEvent & event)
{
	wxBitmap * bmp = event.GetBitmap();

	if (!bmp)
		return;

	do
	{
		if (m_Notebook->GetSelection() != event.GetInt()) // CameraID
			break;

		IFloorImagePreprocessor * camera = (IFloorImagePreprocessor *)m_Notebook->GetPage(event.GetInt())->GetClientData();
		if (!camera)
			break;

		Filter * filter = camera->GetFilter(event.GetString());
		if (!filter)
			break;

		FilterPreviewWindow * preview = m_PreviewMap[filter];
		if (!preview)
			break;
		preview->SetBitmap(*bmp);
	} while (false);
}

void MultiCamSettingsPanel::ReadCameraSettings()
{
	MotionDetectorMultiCam * detector = (MotionDetectorMultiCam *)m_Detector;
	if (!detector->IsRunning())
		return;
	m_CameraSettings.Clear();
	for (int i = 0; i < detector->GetSupportedCamerasCount(); ++i)
	{
		IFloorImagePreprocessorSettings * settings = new IFloorImagePreprocessorSettings();
		m_CameraSettings.Add(settings);
		IFloorImagePreprocessor * camera = detector->GetCameraImagePreprocessor(i);
		if (!camera)
			continue;
		ChainOfFilters & filters = camera->GetFilters();
		for (size_t j = 0; j < filters.size(); ++j)
		{
			Filter * filter = filters[j];
			if (!filter)
				continue;
			wxString config = SerializableBase::Serialize(*filter);
			settings->SetFilterConfig(filter->GetClassInfo()->GetClassName(), config);
		}
	}
}

bool MultiCamSettingsPanel::TransferDataFromWindow()
{
	MotionDetectorMultiCam * detector = (MotionDetectorMultiCam *)m_Detector;
	m_cbMaxBlobArea->GetValidator()->TransferFromWindow();
	m_cbMinBlobArea->GetValidator()->TransferFromWindow();
	m_cbMaxBlobCount->GetValidator()->TransferFromWindow();
	if (!detector->IsRunning())
		return true;
	return wxWindow::TransferDataFromWindow();
}
