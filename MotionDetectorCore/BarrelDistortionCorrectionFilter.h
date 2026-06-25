// Filter description
// Name: Barrel Distortion Correction Filter
// Purpose: Correcting the barrel distortion of the lens
// Original author: Laurence Muller (aka Falcon4ever)

#ifndef __TOUCHSCREEN_FILTER_BARRELDISTORTIONCORRECTION__
#define __TOUCHSCREEN_FILTER_BARRELDISTORTIONCORRECTION__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

class MOTION_DETECTOR_CORE_API BarrelDistortionCorrectionFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(BarrelDistortionCorrectionFilter)
public:
	BarrelDistortionCorrectionFilter();
	virtual ~BarrelDistortionCorrectionFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

private:
	cv::Mat undistorted_with_border( const cv::Mat& image, const cv::Mat& intrinsic, const cv::Mat& distortion, short int border );
	cv::Mat undistorted_with_border2( const  cv::Mat& image, const cv::Mat& intrinsic, const cv::Mat& distortion, short int border );		
	cv::Mat camera;
	cv::Mat dist_coeffs;
	short int border_size;

	cv::Mat b_intrinsic;
	cv::Mat bordered;
	cv::Mat bordered_corr;

	bool init;

	// Method 2
	bool init2;
	cv::Mat MapX;
	cv::Mat MapY;
};

#endif // __TOUCHSCREEN_FILTER_BARRELDISTORTIONCORRECTION__
