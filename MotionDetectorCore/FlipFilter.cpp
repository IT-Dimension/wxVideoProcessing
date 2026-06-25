#include "stdwx.h"
#include "FlipFilter.h"
#include <IFloorCameraSettings.h>

IMPLEMENT_DYNAMIC_CLASS(FlipFilter, Filter);
FlipFilter::FlipFilter()
{
	bFlipVertically = false;
	bFlipHorizontally = false;
	//XS_SERIALIZE(bFlipVertically, wxT("FlipVertically"));
	//XS_SERIALIZE(bFlipHorizontally, wxT("FlipHorizontally"));

}

FlipFilter::~FlipFilter()
{
}

wxString FlipFilter::GetName() const
{
	return _("Flip");
}
#if defined(HAVE_CUDA)
bool FlipFilter::KernelGPU()
{
	if (m_src->empty())
		return false;

	int flipMode = 0;

	if (bFlipVertically && !bFlipHorizontally)
		flipMode = 0;
	else if (!bFlipVertically && bFlipHorizontally)
		flipMode = 1;
	else if (bFlipVertically && bFlipHorizontally)
		flipMode = -1;
	else 
		return false;
	cv::cuda::flip(*m_src, m_dst, flipMode);
	return true;
}
#endif

bool FlipFilter::Kernel()
{
	// derived class responsible for allocating storage for filtered image
	if (source.empty())
		return false;

	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}
	int flipMode = 0;

	if (bFlipVertically && !bFlipHorizontally)
		flipMode = 0;
	else if (!bFlipVertically && bFlipHorizontally)
		flipMode = 1;
	else if (bFlipVertically && bFlipHorizontally)
		flipMode = -1;
	else
	{
		source.copyTo(destination);
		return true;
	};

	cv::flip(source, destination, flipMode);
	return true;
}

void FlipFilter::CreateParamInputs()
{
	//AddParameterBoolInput(_("Flip image vertically"), &bFlipVertically);
	//AddParameterBoolInput(_("Flip image horizontally"), &bFlipHorizontally);
}

bool FlipFilter::IsEnabled()
{
	return m_bEnabled && (bFlipVertically || bFlipHorizontally);
}

void FlipFilter::SetSettings(const IFloorCameraSettings & settings)
{
	bFlipHorizontally = settings.bHorizontalMirror;
	bFlipVertically = settings.bVerticalMirror;
}

wxFORCE_LINK_THIS_MODULE(FlipFilter);