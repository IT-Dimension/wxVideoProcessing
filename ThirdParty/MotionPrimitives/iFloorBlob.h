#ifndef _IFLOORBLOB_H
#define _IFLOORBLOB_H

#include "iFloorMotionTypes.h"
#include <vector>

class iFloorBlob
{
public:
	int					id;		//blob id
	float				area;	//the area of the blob
	float				angle;  
	iFloorPoint			centroid;
	iFloorPoint			lastCentroid;
	iFloorRectangle		boundingRect;
	iFloorRectangle		angleBoundingRect;
	std::vector<iFloorPoint>	pts;	// the contour of the blob

	//----------------------------------------
	iFloorBlob();
	iFloorBlob(const iFloorBlob& blob);
	iFloorBlob& operator=(const iFloorBlob& blob);
	virtual ~iFloorBlob();

	void CopyFrom(const iFloorBlob& blob);
};

typedef std::vector<iFloorBlob> iFloorBlobVector;

#endif // _IFLOORBLOB_H
