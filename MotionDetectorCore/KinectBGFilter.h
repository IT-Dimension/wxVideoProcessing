#ifndef __TOUCHLIB_FILTER_KINECTBG__
#define __TOUCHLIB_FILTER_KINECTBG__

#include "FilterTemplate.h"
#include <mmsystem.h>

class MOTION_DETECTOR_CORE_API KinectBGFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(KinectBGFilter)
public:
	KinectBGFilter();
	virtual ~KinectBGFilter();

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
	bool m_bTrackDark;
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

#endif // __TOUCHLIB_FILTER_KINECTBG__
