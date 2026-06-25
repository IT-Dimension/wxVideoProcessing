#pragma once

#include <iFloorBlob.h>

#define JSON_ACTION_BLOBS wxT("blobs")

class BlobToJSONFormatter
{
public:
	static wxString Format(iFloorBlobVector & blobs);
};