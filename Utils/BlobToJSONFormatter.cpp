#include "stdwx.h"
#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include "JSONObjectFactory.h"
#include "BlobToJSONFormatter.h"

wxString BlobToJSONFormatter::Format(iFloorBlobVector & blobs)
{
	wxString result;

	wxJSONValue root = JSONObjectFactory::Create(JSON_ACTION_BLOBS);
	if (blobs.size() > 0)
	{
		// This should resize the array of items inside root["b"]
		auto & blobRoot = root["b"];
		auto & tmp = blobRoot[blobs.size() - 1];

		#pragma omp parallel for shared(root)
		for (auto i = 0; i < blobs.size(); i++)
		{
			auto & item = blobRoot[i];
			item["c"]["x"] = static_cast<int>(blobs[i].centroid.x);
			item["c"]["y"] = static_cast<int>(blobs[i].centroid.y);
			item["l"]["x"] = static_cast<int>(blobs[i].lastCentroid.x);
			item["l"]["y"] = static_cast<int>(blobs[i].lastCentroid.y);
			auto pointsCount = blobs[i].pts.size();
			// This should resize the array of point coords
			auto & pointRoot = item["p"];
			auto & tmp1 = pointRoot[pointsCount - 1];
			for (auto j = 0; j < pointsCount; ++j)
			{
				pointRoot[j]["x"] = static_cast<int>(blobs[i].pts[j].x);
				pointRoot[j]["y"] = static_cast<int>(blobs[i].pts[j].y);
			}
		}
	}
	else
	{
		wxJSONValue null;
		root["b"] = null;
	}
	wxJSONWriter writer(wxJSONWRITER_NONE);
	writer.Write(root, result);
	result.Replace(" ", "");
	return result;
}