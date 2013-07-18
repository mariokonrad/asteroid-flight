#ifndef __IMAGELOADER_H__
#define __IMAGELOADER_H__

#include "Image.h"

class ImageLoader
{
	public:
		enum Type
		{
			AUTO_DETECT = 0,
			BMP
		};
	public:
		static Image * load(const char *, Type = AUTO_DETECT);
};

#endif
