#include "stdwx.h"
#include "ScalerFilter.h"
#include "Image.h"

IMPLEMENT_DYNAMIC_CLASS(ScalerFilter, Filter);

ScalerFilter::ScalerFilter()
{
	level = (unsigned int) DEFAULT_RECTIFYLEVEL;
}

ScalerFilter::~ScalerFilter()
{
}

wxString ScalerFilter::GetName() const
{
	return _("Scaler");
}

//void ScalerFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("level")] = toString(level);
//}
//
//void ScalerFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "level") == 0)
//	{
//		level = (int) atof(value);
//		level_slider = level;
//		if(show)
//			cvSetTrackbarPos("level", this->name.c_str(), level);
//	}
//}

bool ScalerFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}

	float scale = (float)level / 128.0f;
	destination = source.mul(source) * scale;
	return true;
}

wxFORCE_LINK_THIS_MODULE(ScalerFilter);