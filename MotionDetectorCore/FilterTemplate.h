#ifndef __TOUCHLIB_FILTER__
#define __TOUCHLIB_FILTER__

#if defined(__WXMSW__)
#include <opencv2/opencv.hpp>
#include <SerializableBase.h>
#include "MotionDetectorCorePlugin.h"
#if defined(HAVE_CUDA)
#include <opencv2/core/cuda.hpp>
#endif
#include <wx/link.h>
// IMPORTANT NOTE:
// Linker will discard entire object file without this:
// wxFORCE_LINK_THIS_MODULE(FilterName);
// So add this line with the correct filter class name to the bottom of the cpp file 
// with the filter class definition
//
// Also add line
// wxFORCE_LINK_MODULE(FilterName);
// to the FilterFactory.cpp file to keep linker from discarding the filter

// Preview size
#define PREVIEW_WIDTH 120
#define PREVIEW_HEIGHT 90

class FilterFactory;

typedef void (*FuncGetImage)(int cameraID, const wxString & filterName, cv::Mat image, void * userData);

class MOTION_DETECTOR_CORE_API Filter: public xsSerializable
{
	DECLARE_ABSTRACT_CLASS(Filter)
	friend class FilterFactory;
public:
	Filter();
	virtual ~Filter();

	void Init();

	cv::Mat Process(cv::Mat frame);

	virtual bool Kernel() = 0;
#if defined(HAVE_CUDA)
	cv::cuda::GpuMat& ProcessGPU(cv::cuda::GpuMat& frameMat);
	virtual bool KernelGPU(){return true;}
#endif
	virtual wxString GetName() const = 0;
	virtual bool IsEnabled() { return m_bEnabled; }
	virtual bool IsMandatory() { return false; }
	
	void ConnectTo(Filter * chainedFilter);
	cv::Mat GetOutput() { return m_ResultImage; }
	void SetImageCallback(const FuncGetImage & val, int cameraID, void * userData = NULL);

	void CreatePreviewAndParams(wxWindow * parent, wxSizer * parentSizer, wxWindow * previewWindow);
	void SetEnabled(bool enabled){m_bEnabled = enabled;}

protected:
	bool m_bEnabled;
	bool bUseGPU;
	cv::Mat source;
	cv::Mat destination;
#if defined(HAVE_CUDA)
	cv::cuda::GpuMat* m_src;
	cv::cuda::GpuMat m_dst;
	cv::cuda::GpuMat m_srcGpu;
	cv::Mat m_resMat;
#endif

	Filter * m_ChainedFilter;
	FuncGetImage m_FuncImageCallback;
	void * m_CallbackData;
	int m_CameraID;

	//Parameters setting panel
	void AddParameterSpinInput(wxString name, int * param, int maxValue = 255, int minValue = 0); 
	wxCheckBox * AddParameterBoolInput(wxString name, bool * param);
	virtual void CreateParamInputs();
	void OnCheckBoxClicked(wxCommandEvent & event);
	void OnSpinChanged(wxSpinEvent & event);

	void TransferFromWindow(wxObject * obj);

	wxWindow * m_ParentPanel;
	wxSizer * m_ParamsContainerSizer;

private:
	cv::Mat m_ResultImage;
#if defined(HAVE_CUDA)
	cv::Mat dstMat;
#endif

};

#endif  //__TOUCHLIB_FILTER__
#endif // __WXMSW__
