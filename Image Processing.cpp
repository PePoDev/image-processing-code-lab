#include <iostream>
#include "ImageManager.h"

using namespace std;

int main()
{
	ImageManager* im = new ImageManager();
	im->read("images/mandril.bmp");

	im->convertToRed();
	im->write("images/red.bmp");

	im->restoreToOriginal();
	im->convertToGreen();
	im->write("images/green.bmp");

	im->restoreToOriginal();
	im->convertToBlue();
	im->write("images/blue.bmp");

	im->restoreToOriginal();
	im->convertToGrayscale();
	im->write("images/grayscale.bmp");

	im->restoreToOriginal();
	im->adjustBrightness(100);
	im->write("images/mandril+100.bmp");

	im->restoreToOriginal();
	im->adjustBrightness(-100);
	im->write("images/mandril-100.bmp");

	im->restoreToOriginal();
	im->invert();
	im->write("images/invert.bmp");

	im->restoreToOriginal();
	im->LogTransformation();
	im->write("images/grayscale-log-transformation.bmp");

	im->restoreToOriginal();
	im->PowerLawTransformation(2.2f);
	im->write("images/grayscale-power-law-transformation-2-2.bmp");

	im->restoreToOriginal();
	im->PowerLawTransformation(0.4f);
	im->write("images/grayscale-power-law-transformation-0-4.bmp");

	im->restoreToOriginal();
	im->adjustContrast(100);
	im->write("images/contrast+100.bmp");

	im->restoreToOriginal();
	im->adjustContrast(-100);
	im->write("images/contrast-100.bmp");
	return 0;
}

