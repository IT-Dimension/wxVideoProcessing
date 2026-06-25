#include "stdwx.h"
#include "DummyFilter.h"

IMPLEMENT_DYNAMIC_CLASS(DummyFilter, Filter);

DummyFilter::DummyFilter()
{
}

DummyFilter::~DummyFilter()
{
}

wxString DummyFilter::GetName() const
{
	return _("Camera Preview");
}

bool DummyFilter::Kernel()
{
	return false;
}

void DummyFilter::CreateParamInputs()
{
}

wxFORCE_LINK_THIS_MODULE(DummyFilter);