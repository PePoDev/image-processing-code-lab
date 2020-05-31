#include "ImageManager.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

ImageManager::ImageManager()
{
	header = new unsigned char[BMP_HEADER_SIZE];
	colorTable = new unsigned char[BMP_COLOR_TABLE_SIZE];
	width = 0;
	height = 0;
	bitDepth = 0;
	buf = NULL;
	original = NULL;
}

ImageManager::~ImageManager()
{
	delete[] header;
	delete[] colorTable;
	delete[] buf;
}

bool ImageManager::read(const char* fileName)
{
	FILE* fi = fopen(fileName, "rb");
	if (fi == (FILE*)0)
	{
		cout << "Unable to open file" << endl;
		return false;
	}
	for (int i = 0; i < BMP_HEADER_SIZE; i++)
	{
		header[i] = getc(fi);
	}

	width = (int*)&header[18];
	height = (int*)&header[22];
	bitDepth = (int*)&header[28];
	if (*bitDepth <= 8)
	{
		fread(colorTable, sizeof(unsigned char), BMP_COLOR_TABLE_SIZE, fi);
	}
	buf = new unsigned char[((*height) * (*width) * ((*bitDepth) / BYTE))];

	fread(buf, sizeof(unsigned char), ((*height) * (*width) * ((*bitDepth) / BYTE)), fi);

	original = new unsigned char[((*height) * (*width) * ((*bitDepth) / BYTE))];
	for (int i = 0; i < ((*height) * (*width) * ((*bitDepth) / BYTE)); i++)
	{
		original[i] = buf[i];
	}

	cout << "Image " << fileName << " with " << *width << " x " << *height << " pixels(" << *bitDepth << " bits per pixel) has been read!" << endl;

	fclose(fi);
	return true;
}

bool ImageManager::write(const char* fileName)
{
	FILE* fo = fopen(fileName, "wb");
	if (fo == (FILE*)0)
	{
		cout << "Unable to create file" << endl;
		return false;
	}
	fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, fo);
	if (*bitDepth <= 8)
	{
		fwrite(colorTable, sizeof(unsigned char), BMP_COLOR_TABLE_SIZE, fo);
	}

	fwrite(buf, sizeof(unsigned char), ((*height) * (*width) * ((*bitDepth) / BYTE)), fo);

	cout << "Image " << fileName << " has been written!" << endl;
	fclose(fo);
	return true;
}

void ImageManager::CopyImageHeader(const char* fileName)
{
	FILE* fi = fopen(fileName, "rb");
	if (fi == (FILE*)0)
	{
		cout << "Unable to open file" << endl;
		return;
	}
	for (int i = 0; i < BMP_HEADER_SIZE; i++)
	{
		header[i] = getc(fi);
	}

	width = (int*)&header[18];
	height = (int*)&header[22];
	bitDepth = (int*)&header[28];

	cout << "Image " << fileName << " with " << *width << " x " << *height << " pixels(" << *bitDepth << " bits per pixel) has been read!" << endl;

	fclose(fi);
	return;
}

void ImageManager::CreateBarcodeFrame(int bgColor)
{
	*(int*)&header[18] = 1000;
	*(int*)&header[22] = 300;
	*(int*)&header[28] = 24;

	width = (int*)&header[18];
	height = (int*)&header[22];
	bitDepth = (int*)&header[28];

	buf = new unsigned char[((*height) * (*width) * ((*bitDepth) / BYTE))];

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			setRGB(x, y, bgColor);
		}
	}

	cout << "Created new frame with size " << *width << " * " << *height << endl;
}

void ImageManager::DrawVerticalLine(int start, int lenght, int weight, int color)
{
	for (int i = 0; i < weight; i++) {
		for (int j = 0; j < lenght; j++)
		{
			setRGB(start, 50 + j, color);
		}
		start += 1;
	}
}

void ImageManager::DrawBarcodeCharecter(int code, int seed)
{
	int a[11], i;
	for (i = 0; code > 0; i++)
	{
		a[i] = code % 2;
		code = code / 2;
	}
	for (i = i - 1; i >= 0; i--)
	{
		cout << a[i];
		DrawVerticalLine(seed, 200, 4, (a[i] == 1 ? 0x000000 : 0xffffff1));
		seed += 4;
	}
	cout << endl;
}

void ImageManager::CreateBarcode(const char* code) {
	int seed = 50;
	const int weight = 4;

	DrawVerticalLine(seed, 200, weight * 2, 0x000000);
	seed += weight * 2;
	DrawVerticalLine(seed, 200, weight, 0xffffff);
	seed += weight;
	DrawVerticalLine(seed, 200, weight, 0x000000);
	seed += weight;

	const char* p = &code[0];
	while (*p != '\0') {
		cout << p[0] << endl;
		DrawBarcodeCharecter(GetBarcodeCharecter(p[0]), seed);
		seed += 11 * weight;
		++p;
	}

	DrawVerticalLine(seed, 200, weight, 0x000000);
	seed += weight;
	DrawVerticalLine(seed, 200, weight, 0xffffff);
	seed += weight;
	DrawVerticalLine(seed, 200, weight * 2, 0x000000);
}

int ImageManager::GetBarcodeCharecter(char c) {
	switch (c)
	{
	case ' ':
		return 0b11011001100;
	case '!':
		return 0b11001101100;
	case '"':
		return 0b11001100110;
	case '#':
		return 0b10010011000;
	case '$':
		return 0b10010001100;
	case '%':
		return 0b10001001100;
	case '&':
		return 0b10011001000;
	case '\'':
		return 0b10011000100;
	case '(':
		return 0b10001100100;
	case ')':
		return 0b11001001000;
	case '*':
		return 0b11001000100;
	case '+':
		return 0b11000100100;
	case ',':
		return 0b10110011100;
	case '-':
		return 0b10011011100;
	case '.':
		return 0b10011001110;
	case '/':
		return 0b10111001100;
	case '0':
		return 0b10011101100;
	case '1':
		return 0b10011100110;
	case '2':
		return 0b11001110010;
	case '3':
		return 0b11001011100;
	case '4':
		return 0b11001001110;
	case '5':
		return 0b11011100100;
	case '6':
		return 0b11001110100;
	case '7':
		return 0b11101101110;
	case '8':
		return 0b11101001100;
	case '9':
		return 0b11100101100;
	case ':':
		return 0b11100100110;
	case ';':
		return 0b11101100100;
	case '<':
		return 0b11100110100;
	case '=':
		return 0b11100110010;
	case '>':
		return 0b11011011000;
	case '?':
		return 0b11011000110;
	case '@':
		return 0b11000110110;
	case 'A':
		return 0b10100011000;
	case 'B':
		return 0b10001011000;
	case 'C':
		return 0b10001000110;
	case 'D':
		return 0b10110001000;
	case 'E':
		return 0b10001101000;
	case 'F':
		return 0b10001100010;
	case 'G':
		return 0b11010001000;
	case 'H':
		return 0b11000101000;
	case 'I':
		return 0b11000100010;
	case 'J':
		return 0b10110111000;
	case 'K':
		return 0b10110001110;
	case 'L':
		return 0b10001101110;
	case 'M':
		return 0b10111011000;
	case 'N':
		return 0b10111000110;
	case 'O':
		return 0b10001110110;
	case 'P':
		return 0b11101110110;
	case 'Q':
		return 0b11010001110;
	case 'R':
		return 0b11000101110;
	case 'S':
		return 0b11011101000;
	case 'T':
		return 0b11011100010;
	case 'U':
		return 0b11011101110;
	case 'V':
		return 0b11101011000;
	case 'W':
		return 0b11101000110;
	case 'X':
		return 0b11100010110;
	case 'Y':
		return 0b11101101000;
	case 'Z':
		return 0b11101100010;
	case '[':
		return 0b11100011010;
	case '\\':
		return 0b11101111010;
	case ']':
		return 0b11001000010;
	case '^':
		return 0b11110001010;
	case '_':
		return 0b10100110000;
	}
}

int ImageManager::getRGB(int x, int y)
{
	int i = y * (*width) * ((*bitDepth) / BYTE) + x * ((*bitDepth) / BYTE);
	int b = buf[i];
	int g = buf[i + 1];
	int r = buf[i + 2];
	int color = (r << 16) | (g << 8) | b;
	return color;
}

int ImageManager::getOriginRGB(int x, int y)
{
	int i = y * (*width) * ((*bitDepth) / BYTE) + x * ((*bitDepth) / BYTE);
	int b = original[i];
	int g = original[i + 1];
	int r = original[i + 2];
	int color = (r << 16) | (g << 8) | b;
	return color;
}

void ImageManager::setRGB(int x, int y, int color)
{
	int r = (color >> 16) & 0xff;
	int g = (color >> 8) & 0xff;
	int b = color & 0xff;
	int i = y * (*width) * ((*bitDepth) / BYTE) + x * ((*bitDepth) / BYTE);
	buf[i] = b;
	buf[i + 1] = g;
	buf[i + 2] = r;
}

void ImageManager::restoreToOriginal()
{
	for (int i = 0; i < ((*height) * (*width) * ((*bitDepth) / BYTE)); i++)
	{
		buf[i] = original[i];
	}
}

void ImageManager::convertToRed()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;
			color = (r << 16) | (0 << 8) | 0;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::convertToGreen()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;
			color = (0 << 16) | (g << 8) | 0;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::convertToBlue()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;
			color = (0 << 16) | (0 << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::convertToGrayscale()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = color >> 16 & 0xff;
			int g = color >> 8 & 0xff;
			int b = color & 0xff;
			int newPixelColor = (r + g + b) / 3;
			r = newPixelColor;
			g = newPixelColor;
			b = newPixelColor;
			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::adjustBrightness(int brightness)
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			r = r + brightness;
			r = r > 255 ? 255 : r;
			r = r < 0 ? 0 : r;
			g = g + brightness;
			g = g > 255 ? 255 : g;
			g = g < 0 ? 0 : g;
			b = b + brightness;
			b = b > 255 ? 255 : b;
			b = b < 0 ? 0 : b;
			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::invert()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			r = 255 - r;
			g = 255 - g;
			b = 255 - b;
			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::LogTransformation()
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			int c = 255;

			r = c * log10(1 + (double)r / (double)255);
			g = c * log10(1 + (double)g / (double)255);
			b = c * log10(1 + (double)b / (double)255);

			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::PowerLawTransformation(double gamma)
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			int c = 255;

			r = c * pow((double)r / (double)255, gamma);
			g = c * pow((double)g / (double)255, gamma);
			b = c * pow((double)b / (double)255, gamma);

			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

int* ImageManager::getGrayscaleHistogram()
{
	convertToGrayscale();
	int* histogram = new int[256];
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int gray = color & 0xff;
			histogram[gray]++;
		}
	}
	restoreToOriginal();
	return histogram;
}

void ImageManager::writeHistogramToCSV(int* histogram, const char* fileName)
{
	ofstream fo;
	fo.open(fileName);
	for (int i = 0; i < 256; i++)
	{
		fo << histogram[i] << ",";
	}
	fo.close();
}

float ImageManager::getContrast()
{
	float contrast = 0;
	int* histogram = getGrayscaleHistogram();
	float avgIntensity = 0;
	float pixelNum = (*width) * (*height);
	for (int i = 0; i < 256; i++)
	{
		avgIntensity += histogram[i] * i;
	}
	avgIntensity /= pixelNum;
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int value = color & 0xff;
			contrast += pow((value)-avgIntensity, 2);
		}
	}
	contrast = (float)sqrt(contrast / pixelNum);
	return contrast;
}

void ImageManager::adjustContrast(int contrast)
{
	float currentContrast = getContrast();
	int* histogram = getGrayscaleHistogram();
	float avgIntensity = 0;
	float pixelNum = (*width) * (*height);
	for (int i = 0; i < 256; i++)
	{
		avgIntensity += histogram[i] * i;
	}
	avgIntensity /= pixelNum;
	float min = avgIntensity - currentContrast;
	float max = avgIntensity + currentContrast;
	float newMin = avgIntensity - currentContrast - contrast / 2;
	float newMax = avgIntensity + currentContrast + contrast / 2;
	newMin = newMin < 0 ? 0 : newMin;
	newMax = newMax < 0 ? 0 : newMax;
	newMin = newMin > 255 ? 255 : newMin;
	newMax = newMax > 255 ? 255 : newMax;
	if (newMin > newMax)
	{
		float temp = newMax;
		newMax = newMin;
		newMin = temp;
	}
	float contrastFactor = (newMax - newMin) / (max - min);
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;
			r = (int)((r - min) * contrastFactor + newMin);
			r = r > 255 ? 255 : r;
			r = r < 0 ? 0 : r;
			g = (int)((g - min) * contrastFactor + newMin);
			g = g > 255 ? 255 : g;
			g = g < 0 ? 0 : g;
			b = (int)((b - min) * contrastFactor + newMin);
			b = b > 255 ? 255 : b;
			b = b < 0 ? 0 : b;
			color = (r << 16) | (g << 8) | b;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::averagingFilter(int size)
{
	if (size % 2 == 0)
	{
		cout << "Size Invalid: must be odd number!" << endl;
		return;
	}
	int* tempBuf = new int[(*height) * (*width)];
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int sumRed = 0, sumGreen = 0, sumBlue = 0;
			for (int i = y - size / 2; i <= y + size / 2; i++)
			{
				for (int j = x - size / 2; j <= x + size / 2; j++)
				{
					if (i >= 0 && i < (*height) && j >= 0 && j < (*width))
					{
						int color = getRGB(j, i);
						int r = (color >> 16) & 0xff;
						int g = (color >> 8) & 0xff;
						int b = color & 0xff;
						sumRed += r;
						sumGreen += g;
						sumBlue += b;
					}
				}
			}
			sumRed /= (size * size);
			sumRed = sumRed > 255 ? 255 : sumRed;
			sumRed = sumRed < 0 ? 0 : sumRed;
			sumGreen /= (size * size);
			sumGreen = sumGreen > 255 ? 255 : sumGreen;
			sumGreen = sumGreen < 0 ? 0 : sumGreen;
			sumBlue /= (size * size);
			sumBlue = sumBlue > 255 ? 255 : sumBlue;
			sumBlue = sumBlue < 0 ? 0 : sumBlue;
			int newColor = (sumRed << 16) | (sumGreen << 8) | sumBlue;
			tempBuf[y * (*width) + x] = newColor;
		}
	}
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			setRGB(x, y, tempBuf[y * (*width) + x]);
		}
	}
	delete[] tempBuf;
}

void ImageManager::averagingFilterWithShape(int size, int k)
{
	if (size % 2 == 0)
	{
		cout << "Size Invalid: must be odd number!" << endl;
		return;
	}
	averagingFilter(size);

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			int originColor = getOriginRGB(x, y);
			int originR = (originColor >> 16) & 0xff;
			int originG = (originColor >> 8) & 0xff;
			int originB = originColor & 0xff;

			int maskR = originR - r;
			int maskG = originG - g;
			int maskB = originB - b;

			originR += k * maskR;
			originG += k * maskG;
			originB += k * maskB;

			color = (originR << 16) | (originG << 8) | originB;
			setRGB(x, y, color);
		}
	}
}

void ImageManager::medianFilter(int size)
{
	if (size % 2 == 0)
	{
		cout << "Size Invalid: must be odd number!" << endl;
		return;
	}
	int* tempBuf = new int[(*height) * (*width)];
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int arraySize = size * size;
			int* arrayRed = new int[arraySize];
			int* arrayGreen = new int[arraySize];
			int* arrayBlue = new int[arraySize];
			int sumRed = 0, sumGreen = 0, sumBlue = 0;
			int index = 0;
			for (int i = y - size / 2; i <= y + size / 2; i++)
			{
				for (int j = x - size / 2; j <= x + size / 2; j++)
				{
					if (i >= 0 && i < (*height) && j >= 0 && j < (*width))
					{
						int color = getRGB(j, i);
						int r = (color >> 16) & 0xff;
						int g = (color >> 8) & 0xff;
						int b = color & 0xff;
						arrayRed[index] = r;
						arrayGreen[index] = g;
						arrayBlue[index] = b;
						index++;
					}
				}
			}
			sort(arrayRed, arrayRed + size * size);
			sort(arrayGreen, arrayGreen + size * size);
			sort(arrayBlue, arrayBlue + size * size);

			int centerNumber = (size / 2) + 1;

			sumRed = arrayRed[centerNumber];
			sumRed = sumRed > 255 ? 255 : sumRed;
			sumRed = sumRed < 0 ? 0 : sumRed;
			sumGreen = arrayGreen[centerNumber];
			sumGreen = sumGreen > 255 ? 255 : sumGreen;
			sumGreen = sumGreen < 0 ? 0 : sumGreen;
			sumBlue = arrayBlue[centerNumber];
			sumBlue = sumBlue > 255 ? 255 : sumBlue;
			sumBlue = sumBlue < 0 ? 0 : sumBlue;
			int newColor = (sumRed << 16) | (sumGreen << 8) | sumBlue;
			tempBuf[y * (*width) + x] = newColor;
			delete[] arrayRed;
			delete[] arrayGreen;
			delete[] arrayBlue;
		}
	}
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			setRGB(x, y, tempBuf[y * (*width) + x]);
		}
	}
	delete[] tempBuf;
}

void ImageManager::grayscaleHistogramEqualisation()
{
	int* histogram = new int[256];

	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			int gray = (int)(0.2126 * r + 0.7152 * g + 0.0722 * b);

			histogram[gray]++;
		}
	}

	float pixelNum = (*width) * (*height);

	int cdfMin = 0;
	int* histogramCDF = new int[256];

	for (int i = 0; i < 256; i++)
	{
		histogramCDF[i] = 0;
	}

	for (int i = 0; i < 256; i++)
	{
		if (histogram[i] > 0 && cdfMin == 0)  cdfMin = i;
		if (i == 0)  histogramCDF[i] = histogram[i];
		else  histogramCDF[i] = histogramCDF[i - 1] + histogram[i];
	}

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			int gray = (int)(0.2126 * r + 0.7152 * g + 0.0722 * b);

			gray = (int)(255.0 * (histogramCDF[gray] - cdfMin) / (*width * *height - cdfMin));
			gray = gray > 255 ? 255 : gray;
			gray = gray < 0 ? 0 : gray;

			color = (gray << 16) | (gray << 8) | gray;

			setRGB(x, y, color);
		}
	}
}

void ImageManager::setTemperature(int rTemp, int gTemp, int bTemp)
{
	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			r *= (rTemp / 255.0);
			r = r > 255 ? 255 : r;
			r = r < 0 ? 0 : r;

			g *= (gTemp / 255.0);
			g = g > 255 ? 255 : g;
			g = g < 0 ? 0 : g;

			b *= (bTemp / 255.0);
			b = b > 255 ? 255 : b;
			b = b < 0 ? 0 : b;

			color = (r << 16) | (g << 8) | b;

			setRGB(x, y, color);
		}
	}
}

void ImageManager::colorHistogramEqualisation()
{
	int* histogramRed = new int[256];
	int* histogramGreen = new int[256];
	int* histogramBlue = new int[256];

	for (int i = 0; i < 256; i++)
	{
		histogramRed[i] = 0;
		histogramGreen[i] = 0;
		histogramBlue[i] = 0;
	}

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			histogramRed[r]++;
			histogramGreen[g]++;
			histogramBlue[b]++;
		}
	}

	float pixelNum = (*width) * (*height);

	int histogramRedMin = 0, histogramGreenMin = 0, histogramBlueMin = 0;
	int* histogramRedCDF = new int[256];
	int* histogramGreenCDF = new int[256];
	int* histogramBlueCDF = new int[256];

	for (int i = 0; i < 256; i++)
	{
		histogramRedCDF[i] = 0;
		histogramGreenCDF[i] = 0;
		histogramBlueCDF[i] = 0;
	}

	for (int i = 0; i < 256; i++)
	{
		if (histogramRed[i] > 0 && histogramRedMin == 0)  histogramRedMin = i;
		if (i == 0)  histogramRedCDF[i] = histogramRed[i];
		else  histogramRedCDF[i] = histogramRedCDF[i - 1] + histogramRed[i];

		if (histogramGreen[i] > 0 && histogramGreenMin == 0)  histogramGreenMin = i;
		if (i == 0)  histogramGreenCDF[i] = histogramGreen[i];
		else  histogramGreenCDF[i] = histogramGreenCDF[i - 1] + histogramGreen[i];

		if (histogramBlue[i] > 0 && histogramBlueMin == 0)  histogramBlueMin = i;
		if (i == 0)  histogramBlueCDF[i] = histogramBlue[i];
		else  histogramBlueCDF[i] = histogramBlueCDF[i - 1] + histogramBlue[i];
	}

	for (int y = 0; y < (*height); y++)
	{
		for (int x = 0; x < (*width); x++)
		{
			int color = getRGB(x, y);
			int r = (color >> 16) & 0xff;
			int g = (color >> 8) & 0xff;
			int b = color & 0xff;

			r = (int)(255.0 * (histogramRedCDF[r] - histogramRedCDF[histogramRedMin]) / (pixelNum - histogramRedCDF[histogramRedMin]));
			r = r > 255 ? 255 : r;
			r = r < 0 ? 0 : r;

			g = (int)(255.0 * (histogramGreenCDF[g] - histogramGreenCDF[histogramGreenMin]) / (pixelNum - histogramGreenCDF[histogramGreenMin]));
			g = g > 255 ? 255 : g;
			g = g < 0 ? 0 : g;

			b = (int)(255.0 * (histogramBlueCDF[b] - histogramBlueCDF[histogramBlueMin]) / (pixelNum - histogramBlueCDF[histogramBlueMin]));
			b = b > 255 ? 255 : b;
			b = b < 0 ? 0 : b;

			color = (r << 16) | (g << 8) | b;

			setRGB(x, y, color);
		}
	}
}
