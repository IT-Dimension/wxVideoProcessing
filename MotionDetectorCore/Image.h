#ifndef __TOUCHLIB_IMAGE__
#define __TOUCHLIB_IMAGE__

#include <opencv2/core.hpp>
//#include <touchlib_platform.h>

namespace touchlib
{

	// This class serves as a wrapper to OpenCV's image class
	// to provide a simple interface to pixels and height/width info.

	template<class T> 
	class /*TOUCHLIB_CORE_EXPORT*/ Image  
	{
	public:
		Image(cv::Mat* img = nullptr) { m_img = img; }
		~Image() { m_img = nullptr; }

		int getHeight()
		{
			if (m_img && !m_img->empty())
				return m_img->rows;
			else
				return 0;
		}

		int getWidth()
		{
			if (m_img && !m_img->empty())
				return m_img->cols; 
			else
				return 0;
		}

		void operator=(cv::Mat* img) { m_img = img; }

		inline T* operator[](const int rowIndx)
		{
			return (T*)(m_img->data + rowIndx * m_img->step);
		}

		cv::Mat* m_img;
	};

	typedef struct
	{  
		unsigned char b,g,r;
	} RgbPixel;

	typedef struct
	{  
		float b,g,r;
	} RgbPixelFloat;

	typedef Image<RgbPixel>       RgbImage;
	typedef Image<RgbPixelFloat>  RgbImageFloat;
	typedef Image<unsigned char>  BwImage;
	typedef Image<float>          BwImageFloat;
}

#endif  // __TOUCHLIB_IMAGE__
