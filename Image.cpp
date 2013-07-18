#include "Image.h"

Image::Image()
	: width(0), height(0), data(0)
{}

Image::~Image()
{
	if (data) delete [] data;
}

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

const void * Image::getData()
{
	return (const void *)data;
}
