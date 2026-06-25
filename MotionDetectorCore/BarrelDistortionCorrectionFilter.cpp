// Filter description
// Name: Barrel Distortion Correction Filter
// Purpose: Correcting the barrel distortion of the lens
// Original author: Laurence Muller (aka Falcon4ever)

/*
Tool url:
http://www.multigesture.net/wp-content/uploads/2007/12/touchlib_barreldistortion_tool.zip

example of usage:
<barreldistortioncorrection label="barreldistortioncorrection1">
    <border_size value="20" />
</barreldistortioncorrection>

Place the camera.yml (created by the calibration tool) in the same directory as the config.xml
*/
#include "stdwx.h"
#include "BarrelDistortionCorrectionFilter.h"

IMPLEMENT_DYNAMIC_CLASS(BarrelDistortionCorrectionFilter, Filter);

BarrelDistortionCorrectionFilter::BarrelDistortionCorrectionFilter()
{	
	const char* configfile = "camera.yml";

	cv::FileStorage fs(configfile, cv::FileStorage::READ);
	if (fs.isOpened())
	{
		fs["camera_matrix"] >> camera;
		fs["distortion_coefficients"] >> dist_coeffs;
		fs.release();
	}
	else
	{
		camera = cv::Mat::eye(3, 3, CV_64FC1);
		dist_coeffs = cv::Mat::zeros(5, 1, CV_64FC1);
	}

	border_size = 0;	
	init = false;
	init2 = false;

	MapX = NULL;
	MapY = NULL;
}

BarrelDistortionCorrectionFilter::~BarrelDistortionCorrectionFilter()
{	

}

wxString BarrelDistortionCorrectionFilter::GetName() const
{
	return _("Barrel Distortion Correction");
}

//void BarrelDistortionCorrectionFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("border_size")] = toString(border_size);	
//}
//
//void BarrelDistortionCorrectionFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "border_size") == 0)
//	{
//		border_size = (int) atof(value);		
//	}
//}

bool BarrelDistortionCorrectionFilter::Kernel()
{
	if (border_size > 0)
	{
		cv::Mat tempResult = undistorted_with_border2(source, camera, dist_coeffs, border_size);

		if (!tempResult.empty())
		{
			cv::Rect roi(border_size, border_size, source.cols, source.rows);
			tempResult(roi).copyTo(destination);
		}
	}
	else
	{
		cv::Mat tempResult = undistorted_with_border2(source, camera, dist_coeffs, 0);
		if (!tempResult.empty())
		{
			tempResult.copyTo(destination);
		}
	}

	return true;
}

cv::Mat BarrelDistortionCorrectionFilter::undistorted_with_border( const cv::Mat& image, const cv::Mat& intrinsic, const cv::Mat& distortion, short int border)
{
	if (!init)
	{
		if (intrinsic.empty() || distortion.empty())
			return cv::Mat();

		intrinsic.copyTo(b_intrinsic);

		if (b_intrinsic.type() == CV_64FC1) {
			b_intrinsic.at<double>(0, 2) += border;
			b_intrinsic.at<double>(1, 2) += border;
		}
		else {
			b_intrinsic.at<float>(0, 2) += static_cast<float>(border);
			b_intrinsic.at<float>(1, 2) += static_cast<float>(border);
		}

		init = true;
	}

	if (b_intrinsic.empty())
		return cv::Mat();

	cv::copyMakeBorder(image, bordered, border, border, border, border, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::undistort(bordered, bordered_corr, b_intrinsic, distortion);

	return bordered_corr;
}

cv::Mat BarrelDistortionCorrectionFilter::undistorted_with_border2(const cv::Mat& image, const cv::Mat& intrinsic, const cv::Mat& distortion, short int border)
{
	if (!init2)
	{
		if (intrinsic.empty() || distortion.empty())
			return cv::Mat();

		intrinsic.copyTo(b_intrinsic);

		if (b_intrinsic.type() == CV_64FC1) {
			b_intrinsic.at<double>(0, 2) += border;
			b_intrinsic.at<double>(1, 2) += border;
		}
		else {
			b_intrinsic.at<float>(0, 2) += static_cast<float>(border);
			b_intrinsic.at<float>(1, 2) += static_cast<float>(border);
		}

		cv::Size borderedSize(image.cols + 2 * border, image.rows + 2 * border);

		cv::initUndistortRectifyMap(
			b_intrinsic, distortion, cv::Mat(), b_intrinsic,
			borderedSize, CV_32FC1, MapX, MapY
		);

		init2 = true;
	}

	if (MapX.empty() || MapY.empty())
		return cv::Mat();

	cv::copyMakeBorder(image, bordered, border, border, border, border, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::remap(bordered, bordered_corr, MapX, MapY, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	return bordered_corr;
}

wxFORCE_LINK_THIS_MODULE(BarrelDistortionCorrectionFilter);