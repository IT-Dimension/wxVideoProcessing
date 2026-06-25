#include "stdwx.h"
#include "FilterTemplate.h"
#include <wx/valgen.h>
#include <wx/spinctrl.h>

IMPLEMENT_ABSTRACT_CLASS(Filter, xsSerializable);

Filter::Filter()
{
	Init();
}
//#ifdef HAVE_CUDA
//#undef HAVE_CUDA
//#endif
void Filter::Init()
{
#ifdef HAVE_CUDA
	bUseGPU = false;//cv::gpu::getCudaEnabledDeviceCount();
	m_src = NULL;
#else
	bUseGPU = false;
#endif
	
	m_ChainedFilter = NULL;
	m_FuncImageCallback = NULL;
	m_CallbackData = NULL;
	m_bEnabled = true;
	XS_SERIALIZE(m_bEnabled, wxT("Enabled"));
}

Filter::~Filter()
{
#ifdef HAVE_CUDA
	m_dst.release();
	m_srcGpu.release();
	m_resMat.release();
	dstMat.release();
	cvReleaseImage(&m_ResultImage);
#endif
}

#ifdef HAVE_CUDA
cv::cuda::GpuMat& Filter::ProcessGPU(cv::cuda::GpuMat& frameMat)
{
	m_src = &frameMat; 
	if (IsEnabled())
	{
		try
		{
			// Subclasses must implement this abstract method
			if (!this->KernelGPU()){
				frameMat.copyTo(m_dst);
			}
		}
		catch (...)
		{
			// There was error in the filter
			frameMat.copyTo(m_dst);
			throw 1;
		}
	}
	else
		frameMat.copyTo(m_dst);

	// We need to show result of current filter before it was processed by other filters
	if (m_FuncImageCallback)
	{
		cv::Mat resMat;
		m_dst.download(resMat);
		m_FuncImageCallback(m_CameraID, GetClassInfo()->GetClassName(), &(IplImage)resMat, m_CallbackData);
	}

	if (m_ChainedFilter)
		return m_ChainedFilter->ProcessGPU(m_dst);
	return m_dst;
}
#endif

cv::Mat Filter::Process(cv::Mat frame)
{
	source = frame;

#ifdef HAVE_CUDA
//	cv::gpu::GpuMat srcGpu;
	m_srcGpu.upload(source);
	m_dst = ProcessGPU(m_srcGpu);
	m_dst.download(dstMat);
	//m_ResultImage = cvCreateImage(dstMat.size(), dstMat.depth(), dstMat.channels());
	m_ResultImage = dstMat.clone();
	return m_ResultImage;
#else
	m_ResultImage = source; // If the filter is not enabled, then we should pass source image to the next filter 
	if (IsEnabled())
	{
		try
		{
			// Subclasses must implement this abstract method
			if (this->Kernel() && !destination.empty())
				m_ResultImage = destination; 
		}
		catch (std::exception ex)
		{
			wxString str = ex.what();
		}
		catch (...)
		{
			// There was error in the filter
			
		}
	}

	// We need to show result of current filter before it was processed by other filters
	if (m_FuncImageCallback)
	{
		m_FuncImageCallback(m_CameraID, GetClassInfo()->GetClassName(), m_ResultImage, m_CallbackData);
	}

	// Pass the result to the next filter
	if (m_ChainedFilter)
		return m_ChainedFilter->Process(m_ResultImage);

	return m_ResultImage;
#endif
}

void Filter::ConnectTo(Filter * chainedFilter)
{
	m_ChainedFilter = chainedFilter;
}

void Filter::CreatePreviewAndParams(wxWindow * parent, wxSizer * parentSizer, wxWindow * previewWindow)
{
	parent->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

	wxStaticBox * itemStaticBox = new wxStaticBox(parent, wxID_ANY, GetName());
	wxSizer * mainFilterSizer = new wxStaticBoxSizer(itemStaticBox, wxHORIZONTAL);
	parentSizer->Add(mainFilterSizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_ParentPanel = new wxPanel(parent);
	m_ParentPanel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
	mainFilterSizer->Add(m_ParentPanel, 1, wxGROW, 0);
	
	mainFilterSizer = new wxBoxSizer(wxHORIZONTAL);
	m_ParentPanel->SetSizer(mainFilterSizer);

	wxBoxSizer * previewSizer = new wxBoxSizer(wxVERTICAL);
	mainFilterSizer->Add(previewSizer, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

	previewWindow->Reparent(m_ParentPanel);
	previewSizer->Add(previewWindow, 0, wxALIGN_LEFT|wxBOTTOM, 5);

	m_ParamsContainerSizer = new wxBoxSizer(wxVERTICAL);
	mainFilterSizer->Add(m_ParamsContainerSizer, 1, wxGROW, 5);

	wxCheckBox * cb = AddParameterBoolInput(_("Enabled"), &m_bEnabled);
	if (IsMandatory())
		cb->Disable();

	CreateParamInputs();
}

void Filter::CreateParamInputs()
{
}

void Filter::AddParameterSpinInput(wxString name, int * param, int maxValue, int minValue)
{
	wxBoxSizer * paramSizer = new wxBoxSizer(wxHORIZONTAL);
	m_ParamsContainerSizer->Add(paramSizer, 0, wxGROW|wxBOTTOM, 5);

	wxStaticText * paramName = new wxStaticText(m_ParentPanel, wxID_STATIC, name);
	paramSizer->Add(paramName, 0, wxALIGN_LEFT|wxRIGHT, 5);

	wxSpinCtrl * paramValue = new wxSpinCtrl(m_ParentPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, minValue, maxValue, 0);
	paramSizer->Add(paramValue, 0, wxALIGN_LEFT, 5);

	paramValue->SetValidator(wxGenericValidator(param));

	paramValue->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &Filter::OnSpinChanged, this);
}

wxCheckBox * Filter::AddParameterBoolInput(wxString name, bool * param)
{
	wxCheckBox * paramValue = new wxCheckBox(m_ParentPanel, wxID_ANY, name);
	paramValue->SetValue(*param);
	m_ParamsContainerSizer->Add(paramValue, 0, wxBOTTOM, 5);

	paramValue->SetValidator(wxGenericValidator(param));

	paramValue->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &Filter::OnCheckBoxClicked, this);
	return paramValue;
}

void Filter::SetImageCallback(const FuncGetImage & val, int cameraID, void * userData /*= NULL*/)
{
	m_FuncImageCallback = val;
	m_CameraID = cameraID;
	m_CallbackData = userData;
}

void Filter::OnCheckBoxClicked(wxCommandEvent & event)
{
	TransferFromWindow(event.GetEventObject());
}

void Filter::OnSpinChanged(wxSpinEvent & event)
{
	TransferFromWindow(event.GetEventObject());
}

void Filter::TransferFromWindow(wxObject * obj)
{
	wxWindow * win = wxDynamicCast(obj, wxWindow);
	wxCHECK_MSG(win, , wxT("Where is the window?"));
	wxValidator * validator = win->GetValidator();
	if (validator)
		validator->TransferFromWindow();
}