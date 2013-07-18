#ifndef __IMAGE_BMP_H__
#define __IMAGE_BMP_H__

#include "Image.h"

class Image_BMP : public Image
{
	private:
		struct HEADER
		{
			uint16_t type; // magic identifer
			uint32_t size; // file size in bytes
			uint16_t reserved1;
			uint16_t reserved2;
			uint32_t offset; // offset to image data
		} __attribute__ ((packed));
		struct INFOHEADER
		{
			uint32_t size; // header size in bytes
			int32_t width; // width of the image
			int32_t height; // height of the image
			uint16_t planes; // number of color planes
			uint16_t bits; // bits per pixel
			uint32_t compression;	// compression type:
										// 0: no compression
										// 1: 8 bit run length encoding
										// 2: 4 bit run length encoding
										// 3: RGB bitmap with mask
			uint32_t imagesize;	// image size in bytes
			int32_t xres;					// pixels per meter
			int32_t yres;					// pixels per meter
			uint32_t ncolors;		// number of colors
			uint32_t icolors;		// important colors

			RGBA colorpalette[256];		// color palette
		} __attribute__ ((packed));
	protected:
		virtual int load(const char *);
	public:
};

#endif
