#include "stdwx.h"
#include "BrightnessContrastFilter.h"

IMPLEMENT_DYNAMIC_CLASS(BrightnessContrastFilter, Filter);

BrightnessContrastFilter::BrightnessContrastFilter()
{
	brightness = (float) DEFAULT_BRIGHTNESS;
	contrast = (float) DEFAULT_CONTRAST;

	// mapping from input to output values via lookup table, recalculated when brightness or contrast changed
	lutMat = cv::Mat(1, 256, CV_8UC1);
	
	brightness_slider = 128;
	contrast_slider = 128;

	updateLUT();

	XS_SERIALIZE(brightness_slider, wxT("Brightness"));
	XS_SERIALIZE(contrast_slider, wxT("Contrast"));
}


BrightnessContrastFilter::~BrightnessContrastFilter()
{

}

wxString BrightnessContrastFilter::GetName() const
{
	return _("Brightness/Contrast");
}

void BrightnessContrastFilter::setBrightness(float value)
{
	brightness = value;
	brightness_slider = static_cast<int>((value * 128.0f) + 128.0f);
	updateLUT();
}


void BrightnessContrastFilter::setContrast(float value)
{
	contrast = value;
	contrast_slider = static_cast<int>((value * 128.0f) + 128.0f);
	updateLUT();
}


//void BrightnessContrastFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("brightness")] = toString(brightness);
//	pMap[std::string("contrast")] = toString(contrast);
//}
//
//
//void BrightnessContrastFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "brightness") == 0)
//	{
//		setBrightness(atof(value));
//	} else if(strcmp(name, "contrast") == 0)
//	{
//		setContrast(atof(value));
//	}
//
//}


void BrightnessContrastFilter::updateLUT( void )
{

	float tmp_brightness = 2.0f * ((static_cast<float>(brightness_slider) / 255.0f));
	float tmp_contrast = 2.0f * ((static_cast<float>(contrast_slider) / 255.0f));

	if (brightness != tmp_brightness || contrast != tmp_contrast)
	{
		brightness = tmp_brightness;
		contrast = tmp_contrast;

		if (contrast > 0)
		{
			float delta = 127.0f * contrast;
			float a = 255.0f / (255.0f - delta * 2.0f);
			float b = a * (brightness * 100.0f - delta);
			for (int i = 0; i < 256; i++)
			{
				int v = cvRound(a * i + b);
				if (v < 0) v = 0;
				if (v > 255) v = 255;
				lutMat.at<uchar>(i) = static_cast<uchar>(v);
			}
		}
		else
		{
			float delta = -128.0f * contrast;
			float a = (256.0f - delta * 2.0f) / 255.0f;
			float b = a * brightness * 100.0f + delta;
			for (int i = 0; i < 256; i++)
			{
				int v = cvRound(a * i + b);
				if (v < 0) v = 0;
				if (v > 255) v = 255;
				lutMat.at<uchar>(i) = static_cast<uchar>(v);
			}
		}
	}

}
#if defined(HAVE_CUDA)
bool BrightnessContrastFilter::KernelGPU()
{
	updateLUT();
	cv::cuda::lookUpTable(*m_src, lutMat, m_dst);
	return true;
}
#endif

bool BrightnessContrastFilter::Kernel()
{

	if (source.empty())
		return false;

	updateLUT();
	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	cv::LUT(source, lutMat, destination);
	return true;
}

void BrightnessContrastFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Brightness:"), &brightness_slider, 127, -127);
	AddParameterSpinInput(_("Contrast:"), &contrast_slider, 127, -127);
}

wxFORCE_LINK_THIS_MODULE(BrightnessContrastFilter);