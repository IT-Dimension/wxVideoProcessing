#include "stdwx.h"
#include "HighpassFilter.h"

IMPLEMENT_DYNAMIC_CLASS(HighpassFilter, Filter);

HighpassFilter::HighpassFilter()
{
	element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));

	filterLevel = 5;
	filterLevel_slider = filterLevel;

	scale = 32;
	scale_slider = scale;

	bErodeDialate = false;
	XS_SERIALIZE(bErodeDialate, wxT("ErodeDialate"));
	XS_SERIALIZE(filterLevel_slider, wxT("HighpassLevel"));
	XS_SERIALIZE(scale_slider, wxT("HighpassScale"));

}


HighpassFilter::~HighpassFilter()
{
   
}

wxString HighpassFilter::GetName() const
{
	return _("Highpass");
}

//void HighpassFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("filter")] = toString(filterLevel);
//	pMap[std::string("scale")] = toString(scale);
//
//	if(noErodeDialate)
//		pMap[std::string("mode")] = "1";
//}
//
//
//void HighpassFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "filter") == 0)
//	{
//		filterLevel = (int) atof(value);
//		filterLevel_slider = filterLevel;
//		if(show)
//			cvSetTrackbarPos("filter", this->name.c_str(), filterLevel);
//	}
//
//	if(strcmp(name, "scale") == 0)
//	{
//		scale = (int) atof(value);
//		scale_slider = scale;
//		if(show)
//			cvSetTrackbarPos("scale", this->name.c_str(), scale);
//	}
//
//	if(strcmp(name, "mode") == 0)
//	{
//		if(strcmp(value, "1") == 0)
//		{
//			noErodeDialate = true;
//
//		}
//	}
//}

bool HighpassFilter::Kernel()
{
	if (source.empty())
		return false;

	filterLevel = filterLevel_slider;
	scale = scale_slider;

	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	cv::erode(source, destination, element2, cv::Point(-1, -1), 2);
	cv::dilate(destination, destination, element2, cv::Point(-1, -1), 2);
	//cvConvertScale(source, outra);
	////CV_MEDIAN
	//cvSmooth(outra, outra2, CV_GAUSSIAN, (filterLevel * 2) + 3, (filterLevel * 2) + 3, 0, 0);

	//cvSub(outra, outra2, outra2);

	//if (bErodeDialate)
	//{
	//	cvConvertScale(outra2, destination, ((double)scale + 1.0), 32);
	//	cvErode(destination, destination, element, 2);
	//	cvSmooth(destination, destination, CV_GAUSSIAN, 7, 7, 0, 0);
	//	cvDilate(destination, destination, element2, 1);
	//	cvDilate(destination, destination, element, 1);
	//}
	//else
	//{
	//	cvConvertScale(outra2, destination, ((double)scale + 1.0), 0);
	//	cvErode(destination, destination, element, 1);
	//	cvSmooth(destination, destination, CV_GAUSSIAN, 11, 11, 0, 0);
	//	cvDilate(destination, destination, element, 1);
	//}
	//TODO: remove this
	//showOutput(false, 0, 0);
	return true;
}

void HighpassFilter::CreateParamInputs()
{
	//AddParameterBoolInput(_("Erode/Dialate"), &bErodeDialate);
	//AddParameterSpinInput(_("Filter level:"), &filterLevel_slider, 255);
	//AddParameterSpinInput(_("Filter scale:"), &scale_slider, 255);
}

//void showOutput(bool value, int windowx, int windowy)
//{
//	wxLogDebug(_("sss"));
//}

wxFORCE_LINK_THIS_MODULE(HighpassFilter);