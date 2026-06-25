#ifndef _MULTICAMSETTINGSPANEL_H
#define _MULTICAMSETTINGSPANEL_H


/*!
 * Includes
 */

////@begin includes
#include "wx/listbook.h"
////@end includes
#include "wx/spinctrl.h"    // the base class
#include <MotionDetectorConfigWindowBase.h>
#include <IFloorImagePreprocessorSettings.h>
#include <opencv2/opencv.hpp>

/*!
 * Forward declarations
 */

////@begin forward declarations
class MultiCamSettingsPanel;
class wxListbook;
////@end forward declarations
class MotionDetectorMultiCam;
class IFloorImagePreprocessor;
class FilterPreviewWindow;
class wxBitmapEvent;

WX_DECLARE_VOIDPTR_HASH_MAP(FilterPreviewWindow *, FilterPreviewMap);

/*!
 * MultiCamSettingsPanel class declaration
 */

class MultiCamSettingsPanel: public MotionDetectorConfigWindowBase
{
	DECLARE_DYNAMIC_CLASS( MultiCamSettingsPanel )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	MultiCamSettingsPanel();
	MultiCamSettingsPanel(MotionDetectorMultiCam * detector, wxWindow * parent);

	/// Creation
	bool Create(MotionDetectorMultiCam * detector, wxWindow * parent);

	/// Destructor
	~MultiCamSettingsPanel();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();
	
	// IFloorConfigureManager implementation
	virtual bool ReadConfig();
	virtual bool SaveConfig();

	virtual void StartDetection();
	virtual void StopDetection();

	virtual bool TransferDataFromWindow();

////@begin MultiCamSettingsPanel event handler declarations

////@end MultiCamSettingsPanel event handler declarations
	void OnImageCallback(wxBitmapEvent & event);

////@begin MultiCamSettingsPanel member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end MultiCamSettingsPanel member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

	void CreateCameraSettings(int index);
	static void ImageCallback(int cameraID, const wxString& filterName, cv::Mat image, void* userData);

	void ReadCameraSettings();

////@begin MultiCamSettingsPanel member variables
	wxListbook* m_Notebook;
	/// Control identifiers
	enum {
		ID_MULTICAMSETTINGSPANEL = 10000,
		ID_DisplayGeometry = 10002,
		ID_LISTBOOK = 10001
	};
////@end MultiCamSettingsPanel member variables
	wxSpinCtrl*  m_cbMaxBlobArea;
	wxSpinCtrl*  m_cbMinBlobArea;
	wxSpinCtrl*  m_cbMaxBlobCount;
	wxCheckBox*	 m_cbUseOneOnMulti;
	enum{
		ID_MAXBLOBAREA = 10003,
		ID_MINBLOBAREA = 10004,
		ID_MAXBLOBCOUNT = 10005,
		ID_USEONE = 10006
	};
	FilterPreviewMap m_PreviewMap;
	IFloorImagePreprocessorSettingsArray m_CameraSettings;
};

#endif // _MULTICAMSETTINGSPANEL_H
