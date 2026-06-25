#include "stdwx.h"
#include "InvertFilter.h"

IMPLEMENT_DYNAMIC_CLASS(InvertFilter, Filter);

InvertFilter::InvertFilter()
{
}

InvertFilter::~InvertFilter()
{
}

wxString InvertFilter::GetName() const
{
	return _("Invert");
}

// The smooth filter really needs the blur size as a param
bool InvertFilter::Kernel()
{
	if (source.empty())
		return false;

	// derived class responsible for allocating storage for filtered image
	if (destination.empty())
	{
		destination = cv::Mat::zeros(source.size(), source.type());
	}

	destination = ~source;
	return true;
}

wxFORCE_LINK_THIS_MODULE(InvertFilter);