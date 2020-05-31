#pragma once
#ifndef __IMAGE_MANAGER_
#define __IMAGE_MANAGER_
#define _CRT_SECURE_NO_DEPRECATE
#define BYTE 8
#define BMP_COLOR_TABLE_SIZE 1024
#define BMP_HEADER_SIZE 54
class ImageManager
{
public:
	ImageManager();
	virtual ~ImageManager();
	bool read(const char* fileName);
	bool write(const char* fileName);
	int getRGB(int x, int y);
	int getOriginRGB(int x, int y);
	void setRGB(int x, int y, int color);
	void restoreToOriginal();
	void convertToRed();
	void convertToGreen();
	void convertToBlue();
	void convertToGrayscale();
	void adjustBrightness(int brightness);
	void LogTransformation();
	void PowerLawTransformation(double gamma);
	void invert();
	int* getGrayscaleHistogram();
	void writeHistogramToCSV(int* histogram, const char* fileName);
	float getContrast();
	void adjustContrast(int contrast);
	void averagingFilter(int size);
	void averagingFilterWithShape(int size, int k);
	void medianFilter(int size);
	void grayscaleHistogramEqualisation();
	void setTemperature(int rTemp, int gTemp, int bTemp);
	void colorHistogramEqualisation();
	void CopyImageHeader(const char* fileName);
	void DrawVerticalLine(int start, int lenght, int weight, int color);
	void CreateBarcodeFrame(int bgColor);
	void DrawBarcodeCharecter(int code, int seed);
	void CreateBarcode(const char* code);
	int GetBarcodeCharecter(char c);
	int* width;
	int* height;
	int* bitDepth;
protected:
private:
	unsigned char* header;
	unsigned char* colorTable;
	unsigned char* buf;
	unsigned char* original;
};
#endif