#include "stdwx.h"
#include "RectifyFilter.h"
#include "Image.h"

IMPLEMENT_DYNAMIC_CLASS(RectifyFilter, Filter);

RectifyFilter::RectifyFilter()
{
    level = (unsigned int) DEFAULT_RECTIFYLEVEL;
	level_slider = level;

	bAutoSet = false;
	XS_SERIALIZE(bAutoSet, wxT("AutoSet"));
	XS_SERIALIZE(level_slider, wxT("Threshold"));
}

RectifyFilter::~RectifyFilter()
{
}

wxString RectifyFilter::GetName() const
{
	return _("Rectify");
}

//void RectifyFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("level")] = bAutoSet ? std::string("auto") : toString(level);
//}
//
//void RectifyFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "level") == 0)
//	{
//		if(strcmp(value, "auto") == 0)
//		{
//			bAutoSet = true;
//			printf("Auto set\n");
//		} else
//		{
//			level = (int) atof(value);
//			level_slider = level;
//			if(show)
//			  cvSetTrackbarPos("level", this->name.c_str(), level);
//		}
//	}
//	
//}

bool RectifyFilter::Kernel()
{
	level = level_slider;
	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}

	if (bAutoSet)
	{
		touchlib::BwImage img(&source);

		int h, w;
		h = img.getHeight();
		w = img.getWidth();

		unsigned char highest = 0;

		for (int y=0; y<h; y++)
			for (int x=0; x<w; x++)
			{
				if (img[y][x] > highest)
					highest = img[y][x];
			}

		setLevel((unsigned int)highest);
		bAutoSet = false;
	}

	cv::threshold(source, destination, level, 255, cv::THRESH_TOZERO);	//CV_THRESH_BINARY
	return true;
}

void RectifyFilter::CreateParamInputs()
{
	AddParameterBoolInput(_("Automatic level"), &bAutoSet);
	AddParameterSpinInput(_("Threshold level:"), &level_slider, 255);
}

wxFORCE_LINK_THIS_MODULE(RectifyFilter);