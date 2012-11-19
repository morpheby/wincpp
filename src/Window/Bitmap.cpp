/*
 * Bitmap.cpp
 *
 *  Created on: 13.5.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "Bitmap.h"
#include <string.h>

Bitmap::Bitmap(HDC dc, HBITMAP bmp) {
	BITMAP bitmap;
	GetObject(bmp, sizeof(BITMAP), &bitmap);

    bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi_.bmiHeader.biWidth = bitmap.bmWidth;
    bmi_.bmiHeader.biHeight = bitmap.bmHeight;
    bmi_.bmiHeader.biPlanes = 1;
    bmi_.bmiHeader.biBitCount = 32;
    bmi_.bmiHeader.biCompression = BI_RGB;
    bmi_.bmiHeader.biSizeImage = 0;
    bmi_.bmiHeader.biXPelsPerMeter = 0;
    bmi_.bmiHeader.biYPelsPerMeter = 0;
    bmi_.bmiHeader.biClrUsed = 0;
    bmi_.bmiHeader.biClrImportant = 0;

    DWORD bmpSize = ((bitmap.bmWidth * bmi_.bmiHeader.biBitCount + 31) / 32) * 4 * bitmap.bmHeight;

    bits_ = malloc(bmpSize);

    GetDIBits(dc, bmp, 0, (UINT)bitmap.bmHeight, bits_, &bmi_, DIB_RGB_COLORS);
}

Bitmap::~Bitmap() {
	free(bits_);
}

Bitmap::Bitmap(const Bitmap &copyFrom) :
			bmi_(copyFrom.bmi_),
			bits_{nullptr} {
	DWORD bmpSize = ((bmi_.bmiHeader.biWidth * bmi_.bmiHeader.biBitCount + 31) / 32) * 4 * bmi_.bmiHeader.biHeight;
	bits_ = malloc(bmpSize);
	memcpy(bits_, copyFrom.bits_, bmpSize);
}

HBITMAP Bitmap::CreateDDB(HDC dc) {
	return CreateDIBitmap(dc, &bmi_.bmiHeader, CBM_INIT,
			bits_, &bmi_, DIB_RGB_COLORS);
}
