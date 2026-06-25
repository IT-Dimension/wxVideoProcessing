// Filter description
// Name: Crop Filter
// Purpose: Allows the user to crop the source image
// Original author: Laurence Muller (aka Falcon4ever)

/*
example of usage:
<crop label="crop">
	<posX value="40" />
	<posY value="40" />
	<height value="120" />
	<width value="160" />
</crop>

posX and posY specifies an offset from upperleft corner.
heigth and width specifies the crop size.
*/
#include "stdwx.h"
#include "CropFilter.h"

IMPLEMENT_DYNAMIC_CLASS(CropFilter, Filter);

CropFilter::CropFilter()
{
	img_rect.x = 0;
	img_rect.y = 0;
	img_rect.height = DEFAULT_CROPHEIGHT;
	img_rect.width = DEFAULT_CROPWIDTH;

	firsttime = true;

	XS_SERIALIZE(img_rect.x, wxT("x"));
	XS_SERIALIZE(img_rect.y, wxT("y"));
	XS_SERIALIZE(img_rect.width, wxT("width"));
	XS_SERIALIZE(img_rect.height, wxT("height"));
}

CropFilter::~CropFilter()
{
}

wxString CropFilter::GetName() const
{
	return _("Crop");
}

//void CropFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("posX")] = toString(img_rect.x);
//	pMap[std::string("posY")] = toString(img_rect.y);
//	pMap[std::string("height")] = toString(img_rect.height);
//	pMap[std::string("width")] = toString(img_rect.width);
//}
//
//void CropFilter::setParameter(const char *name, const char *value)
//{	
//	if(strcmp(name, "posX") == 0)
//	{
//		img_rect.x  = (int) atof(value);
//		level_posX_slider = img_rect.x;
//
//		if(show)
//			cvSetTrackbarPos("posX", this->name.c_str(), level_posX_slider);
//	}
//	else if(strcmp(name, "posY") == 0)
//	{
//		img_rect.y  = (int) atof(value);
//		level_posY_slider = img_rect.x;
//
//		if(show)
//			cvSetTrackbarPos("posY", this->name.c_str(), level_posY_slider);
//	}
//	else if(strcmp(name, "width") == 0)
//	{
//		img_rect.width  = (int) atof(value);
//		level_width_slider = img_rect.width;
//
//		//if(show)
//		//	cvSetTrackbarPos("width", this->name.c_str(), level_width_slider);
//
//		if(destination)
//			cvReleaseImage(&destination);
//
//        destination = cvCreateImage(cvSize(img_rect.width,img_rect.height), 8, 1);
//	}
//	else if(strcmp(name, "height") == 0)
//	{
//		img_rect.height  = (int) atof(value);
//		level_height_slider = img_rect.height;
//
//		//if(show)
//		//	cvSetTrackbarPos("height", this->name.c_str(), level_height_slider);
//
//		if(destination)
//			cvReleaseImage(&destination);
//
//        destination = cvCreateImage(cvSize(img_rect.width,img_rect.height), 8, 1);
//	}	
//}

bool CropFilter::Kernel()
{
	if (source.empty())
		return false;

	if (firsttime)
	{
		max_x = source.cols;
		max_y = source.rows;
		firsttime = false;
	}

	//if (show) 
	{	
/*
		// Todo: Fix filter chain...
		// Status: Currently disabled.
		//
		// Problem:
		// When changing the width and height, a new destination image has to be created.
		// The current destination image should be released first, the a new destination image should be
		// created from the img_rect values.
		// By dynamicly changing the destination size the next filter in the filterchain should adjust
		// all its allocated images aswell (width and height).
		// Currently this isnt done, which causes openCV to choke and crash.
		//
		// Current workaround:
		// Set the width and height manual in the config.xml before starting the configapp or your application

		// Uncomment the following part if dynamic resizing is allowed:
		if(img_rect.width != level_width_slider)
		{	
			if(level_width_slider + img_rect.x < max_x)
				img_rect.width = level_width_slider;

			if(destination)
				cvReleaseImage(&destination);
		}

		if(img_rect.height != level_height_slider)
		{
			if(level_height_slider + img_rect.y < max_y)
				img_rect.height = level_height_slider;

			if(destination)
				cvReleaseImage(&destination);
		}
*/
	}

	if (img_rect.x < 0) img_rect.x = 0;
	if (img_rect.y < 0) img_rect.y = 0;
	if (img_rect.x >= source.cols) img_rect.x = source.cols - 1;
	if (img_rect.y >= source.rows) img_rect.y = source.rows - 1;

	if (img_rect.x + img_rect.width > source.cols)
		img_rect.width = source.cols - img_rect.x;
	if (img_rect.y + img_rect.height > source.rows)
		img_rect.height = source.rows - img_rect.y;

	if (img_rect.width <= 0 || img_rect.height <= 0)
		return false;

	source(img_rect).copyTo(destination);

	return true;
}

wxFORCE_LINK_THIS_MODULE(CropFilter);