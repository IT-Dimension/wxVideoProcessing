// Filter description
// Name: Crop Filter
// Purpose: Allows the user to crop the source image
// Original author: Laurence Muller (aka Falcon4ever)

#ifndef __TOUCHLIB_FILTER_CROP__
#define __TOUCHLIB_FILTER_CROP__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>

#define DEFAULT_CROPWIDTH 640
#define DEFAULT_CROPHEIGHT 480

class MOTION_DETECTOR_CORE_API CropFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(CropFilter)
public:
	CropFilter();
	virtual ~CropFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

private:	
	bool firsttime;
	int max_x;
	int max_y;
	cv::Rect img_rect;
};

#endif // __TOUCHLIB_FILTER_CROP__
