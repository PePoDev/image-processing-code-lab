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

int ImageManager::getRGB(int x, int y)
{
	int i = y * (*width) * ((*bitDepth) / BYTE) + x * ((*bitDepth) / BYTE);
	int b = buf[i];
	int g = buf[i + 1];
	int r = buf[i + 2];
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