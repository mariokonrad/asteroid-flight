#include "ImageLoader.h"
#include "Image_BMP.h"

Image * ImageLoader::load(const char * path, ImageLoader::Type type)
{
	Image * img = 0;
	switch (type) {
		case BMP:
			img = new Image_BMP;
			break;
		default:
			return 0;
	}
	if (!img->load(path)) return img;
	delete img;
	return 0;
}

