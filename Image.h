#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdint.h>

class Image
{
		friend class ImageLoader;
	public:
		struct RGBA
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
	protected:
		int width;
		int height;
		unsigned char * data;
		virtual int load(const char *) = 0;
	public:
		Image();
		virtual ~Image();
		int getWidth();
		int getHeight();
		const void * getData();
};

#endif
