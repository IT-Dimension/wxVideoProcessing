#ifndef __TOUCHLIB_FILTER_BACKGROUND__
#define __TOUCHLIB_FILTER_BACKGROUND__

#include "FilterTemplate.h"
#include <opencv2/core.hpp>
#include <vector>

class MOTION_DETECTOR_CORE_API BackgroundFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(BackgroundFilter)
public:
	BackgroundFilter();
	virtual ~BackgroundFilter();
	
	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

	void setMask(void *aPoints,int xRes, int yRes);
	void clearMask();
protected:
	virtual void CreateParamInputs();
private:
	bool recapture;
	cv::Mat reference;
	bool ownsImage;
	int updateThreshold;		// anything above this threshold is considered a 'press' and not part of the background
	int count;
	int currentRow;
	cv::Mat mask;
	std::vector<cv::Point> polyMask;
	int nPolyMask;
};

#endif // __TOUCHLIB_FILTER_BACKGROUND__
