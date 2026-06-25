#include "stdwx.h"
#include "ResizeFilter.h"

IMPLEMENT_DYNAMIC_CLASS(ResizeFilter, Filter);

ResizeFilter::ResizeFilter()
{
	sizeX = DEFAULT_RESIZEWIDTH;
	sizeY = DEFAULT_RESIZEHEIGHT;
}

ResizeFilter::~ResizeFilter()
{

}

wxString ResizeFilter::GetName() const
{
	return _("Resize");
}

//void ResizeFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("sizeX")] = toString(sizeX);
//	pMap[std::string("sizeY")] = toString(sizeY);
//}
//
//
//void ResizeFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "sizeX") == 0)
//	{
//		sizeX  = (int) atof(value);
//
//		if(destination)
//			cvReleaseImage(&destination);
//
//        destination = cvCreateImage(cvSize(sizeX,sizeY), 8, 1);
//	}
//	if(strcmp(name, "sizeY") == 0)
//	{
//		sizeY  = (int) atof(value);
//
//		if(destination)
//			cvReleaseImage(&destination);
//
//        destination = cvCreateImage(cvSize(sizeX,sizeY), 8, 1);
//	}
//}

// We are assuming 8 bit depth, 1 channel.. so this filter should happen after the mono filter..
bool ResizeFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (sizeX <= 0 || sizeY <= 0)
		return false;

	cv::resize(source, destination, cv::Size(sizeX, sizeY), 0, 0, cv::INTER_LINEAR);

	return true;
}

wxFORCE_LINK_THIS_MODULE(ResizeFilter);