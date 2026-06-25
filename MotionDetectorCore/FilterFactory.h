#ifndef _FILTERFACTORY_H
#define _FILTERFACTORY_H

#include "FilterTemplate.h"

class FilterFactory
{
public:
	static Filter * CreateFilter(const wxString & type);
};

#endif // _FILTERFACTORY_H