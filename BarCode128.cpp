#include <iostream>
#include "ImageManager.h"

using namespace std;

int main()
{
	ImageManager* im = new ImageManager();
	im->read("images/mandril.bmp");

	// midterm code
	/*ImageManager* im = new ImageManager();
	string prefixCode = "59050220:H6S0B0:";
	string input;
	
	im->CopyImageHeader("images/mandril.bmp");
	im->CreateBarcodeFrame(0xffffff);

	cout << "Please enter code: ";
	cin >> input;
	const char* code = (prefixCode.append(input)).c_str();

	im->CreateBarcode(code);
	im->write("images/barcode-128.bmp");*/
}