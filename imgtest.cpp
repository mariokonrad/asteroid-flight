#include "ImageLoader.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char ** argv)
{
	Image * img = ImageLoader::load("data.bmp", ImageLoader::BMP);
	if (!img) {
		cerr << "Could not load texture: data.bmp" << endl;
		exit(-1);
	}

	cout << "img info:"
		<< " width=" << img->getWidth()
		<< " height=" << img->getHeight()
		<< endl;

	delete img;
	return 0;
}
