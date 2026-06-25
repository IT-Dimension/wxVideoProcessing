#ifndef __TOUCHLIB_FILTER_DYNAMICBG__
#define __TOUCHLIB_FILTER_DYNAMICBG__

#include "FilterTemplate.h"
#include <mmsystem.h>
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API DynamicBGFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(DynamicBGFilter)
public:
	DynamicBGFilter();
	virtual ~DynamicBGFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU();
#endif
	virtual wxString GetName() const;

protected:
	void CreateParamInputs();

private:
	cv::Mat floatBgImg;
	cv::Mat floatBgImgTemp;
	cv::Mat grayBg;
#if defined(HAVE_CUDA)
	cv::cuda::GpuMat m_matFloatBgImg;
	cv::cuda::GpuMat m_matFloatBgImgTemp;
	cv::cuda::GpuMat m_matGrayBg;
	bool bStart;
#endif
	bool m_bTrackDark;
	bool m_bLearnBackground;
	int m_ExposureStartTime;
	int m_CameraExposureTime;
	int m_LearnRate;
};

#endif // __TOUCHLIB_FILTER_DYNAMICBG__
