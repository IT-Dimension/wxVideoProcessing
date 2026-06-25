#include "iFloorBlob.h"

iFloorBlob::iFloorBlob()
: id(-1)
, area(0.0f)
, angle(0.0f)
{
}

iFloorBlob::iFloorBlob(const iFloorBlob& blob)
{
	CopyFrom(blob);
}

iFloorBlob& iFloorBlob::operator=(const iFloorBlob& blob)
{
	if (&blob != this)
	{
		CopyFrom(blob);
	}
	return *this;
}

iFloorBlob::~iFloorBlob()
{
}

void iFloorBlob::CopyFrom(const iFloorBlob& blob)
{
	pts = blob.pts;
	id = blob.id;
	area = blob.area;
	angle = blob.angle;  
	boundingRect = blob.boundingRect;
	angleBoundingRect = blob.angleBoundingRect;
	centroid = blob.centroid;
	lastCentroid = blob.lastCentroid;
}