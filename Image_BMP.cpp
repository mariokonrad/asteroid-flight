#include "Image_BMP.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

int Image_BMP::load(const char * path)
{
	FILE * file = fopen(path, "rb");
	if (!file) return -1;

	struct HEADER head;
	struct INFOHEADER info;
	
	// init
	bzero(&head, sizeof(head));
	bzero(&info, sizeof(info));

	// read header
	if (fread(&head, sizeof(unsigned char), sizeof(head), file) != sizeof(head))
	{
		fclose(file);
		return -2;
	}

	// read info
	long info_size = head.offset - sizeof(head);
	if (fread(&info, sizeof(unsigned char), info_size, file) != (size_t)info_size)
	{
		fclose(file);
		return -2;
	}
	
	// read data
	long size = info.imagesize;
	if (size == 0) size = info.width * info.height * ((info.bits + 7) / 8);
	data = new unsigned char[size];
	if (fread(data, sizeof(unsigned char), size, file) != (size_t)size)
	{
		fclose(file);
		delete data; data = 0;
		return -1;
	}
	fclose(file);
	width = info.width; height = info.height;
	return 0;
}

