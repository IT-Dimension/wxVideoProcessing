#ifndef _DUMMYFILTER_H
#define _DUMMYFILTER_H

#include "FilterTemplate.h"

class MOTION_DETECTOR_CORE_API DummyFilter : public Filter
{
	DECLARE_DYNAMIC_CLASS_NO_COPY(DummyFilter)
public:
	DummyFilter();
	virtual ~DummyFilter();

	bool Kernel();
#if defined(HAVE_CUDA)
	bool KernelGPU(){return false;}
#endif
	virtual wxString GetName() const;

	// This filter is always disabled
	virtual bool IsEnabled() { return false; };
	virtual bool IsMandatory() { return true; };

protected:
	void CreateParamInputs();
private:

};

#endif // _DUMMYFILTER_H
