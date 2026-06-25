#include "stdwx.h"
#include "BackgroundFilter.h"
#include "vector2d.h"
//#include "Image.h"

IMPLEMENT_DYNAMIC_CLASS(BackgroundFilter, Filter);

#define ROWSTOSCAN	40

#define UPDATERATE_UP		2
#define UPDATERATE_DOWN		1

#define DEFAULT_UPDATE_THRESH	50

BackgroundFilter::BackgroundFilter()
{
	nPolyMask = 1;
	recapture = false;
	count = -1;
	//recapture = true;
	//count = 10;
	updateThreshold = DEFAULT_UPDATE_THRESH;
	currentRow = 0;
	XS_SERIALIZE(updateThreshold, wxT("Threshold"))
//	pMap["threshold"] = iFloorImageRecognitionParam(INT_PARAM, &updateThreshold, 0, 255);
}

BackgroundFilter::~BackgroundFilter()
{

}

wxString BackgroundFilter::GetName() const
{
	return _("Background");
}

//void BackgroundFilter::setParameter(const char *name, const char *value)
//{
//	if(strcmp(name, "capture") == 0)
//	{
//		printf("Recap\n");
//		recapture = true;	
//	} else if(strcmp(name, "threshold") == 0)
//	{
//		updateThreshold = (int) atof(value);
//	} else if(strcmp(name, "mask") == 0)
//	{
//		if(value)
//			setMask((touchlib::vector2df*)value,GRID_X+1,GRID_Y+1);	
//		else
//			clearMask();
//	} 
//}

//void BackgroundFilter::getParameters(iFloorParamMap& pMap)
//{
//	pMap[std::string("threshold")] = toString(updateThreshold);
//}


//#define ADAPTIVE_BACKGROUND

bool BackgroundFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image

	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), CV_8UC1);
	}

	if (count > -1)
		count--;
	
	if (reference.empty() || recapture || count == 0)
	{
		source.copyTo(reference);

		if (mask.empty()) {
			mask = cv::Mat::zeros(reference.size(), reference.type());
		}

		// Якщо маска задана, малюємо її сучасним методом cv::fillConvexPoly
		if (!polyMask.empty() && updateThreshold == 0) {
			mask.setTo(cv::Scalar(255, 255, 255));
			cv::fillConvexPoly(mask, polyMask, cv::Scalar(0, 0, 0));
		}

		// Сучасне віднімання скаляра (замість cvSubS)
		if (updateThreshold != 0)
		{
			reference -= cv::Scalar(updateThreshold, updateThreshold, updateThreshold);
		}

		recapture = false;

		//count = updateThreshold;
		recapture = false;
	}

#ifdef ADAPTIVE_BACKGROUND
	BwImage imgSrc(source), imgRef(reference);

	int x, y;
	int h, w;
	h = source->height;
	w = source->width;

	int stoprow = currentRow + ROWSTOSCAN;

	if (stoprow > h)
		stoprow = h;

	// only do N number of rows per frame to speed up processing.. 
	for(y=currentRow; y<stoprow; y++)
	{
		for(x=0; x<w; x++)
		{
			int pix, ref;
			pix = imgSrc[y][x];
			ref = imgRef[y][x];
			if (pix - ref < updateThreshold)		// bright spots are assummed to be active fingers, not background..
			{
				if (pix > ref) 
				{
					ref += UPDATERATE_UP;
					if (ref > pix)
						ref = pix;

					imgRef[y][x] = ref;		// update background
				}

				// In most cases we won't really need to go 'down'..
				// as the screen gets dirtier, it gets brighter.. 
				// 
				//if (pix < ref)
					//ref -= UPDATERATE_DOWN;
				
			}

		}
	}
	currentRow += ROWSTOSCAN;

	if (currentRow >= h)
		currentRow = 0;
#endif
	// destination = source-reference
	cv::subtract(source, reference, destination);
	return true;
}

void BackgroundFilter::setMask(void* vaPoints, int xGrid, int yGrid)
{
	touchlib::vector2df* aPoints = (touchlib::vector2df*)vaPoints;

	// Перевиділяємо розмір нашого вектора точок C++
	int totalPoints = 2 * (xGrid + yGrid - 2);
	polyMask.resize(totalPoints);

	int countPoints = 0;
	// top side
	for (int i = 0; i < xGrid; i++) {
		polyMask[countPoints++] = cv::Point(static_cast<int>(aPoints[i].X), static_cast<int>(aPoints[i].Y));
	}
	// right side
	for (int i = 2; i < yGrid; i++) {
		polyMask[countPoints++] = cv::Point(static_cast<int>(aPoints[i * xGrid - 1].X), static_cast<int>(aPoints[i * xGrid - 1].Y));
	}
	// bottom side
	for (int i = xGrid - 1; i >= 0; i--) {
		polyMask[countPoints++] = cv::Point(static_cast<int>(aPoints[i + (yGrid - 1) * xGrid].X), static_cast<int>(aPoints[i + (yGrid - 1) * xGrid].Y));
	}
	// left side
	for (int i = yGrid - 2; i > 0; i--) {
		polyMask[countPoints++] = cv::Point(static_cast<int>(aPoints[i * xGrid].X), static_cast<int>(aPoints[i * xGrid].Y));
	}

	recapture = true;
}// deletes an old mask and tells Kernel func to recapture
void BackgroundFilter::clearMask()
{
	polyMask.clear();
	if (!mask.empty())
	{
		mask.setTo(cv::Scalar(0, 0, 0));
	}
	recapture = true;

}

void BackgroundFilter::CreateParamInputs()
{
	AddParameterSpinInput(_("Update threshold"), &updateThreshold); 
	AddParameterBoolInput(_("Recapture"), &recapture); 
}

wxFORCE_LINK_THIS_MODULE(BackgroundFilter);