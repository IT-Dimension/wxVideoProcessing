#ifndef __TOUCHLIB_FILTER_SUBORBG__
#define __TOUCHLIB_FILTER_SUBORBG__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>
#include <mmsystem.h>

class MOTION_DETECTOR_CORE_API SubOrBGFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(SubOrBGFilter)
public:
	SubOrBGFilter();
	virtual ~SubOrBGFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;
	void SaveBackground(wxString filename);
	void ResetCalibration();
protected:
	void CreateParamInputs();

private:
	cv::Mat background;
	cv::Mat destination1;
	bool m_bLearnBackground;
	int m_ExposureStartTime;
	int m_CameraExposureTime;
	bool m_bCalibrated;
	bool m_bErodeDilate;
	wxString m_filename;
	cv::Mat m_erodeKernel;
	cv::Mat m_dilateKernel;
	bool m_bStart;
#if defined(HAVE_CUDA)
	cv::cuda::GpuMat m_matBg;
#endif

};

#endif // __TOUCHLIB_FILTER_SUBORBG__
