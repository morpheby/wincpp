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
#include <iostream>

#define NEW_DIB_FORMAT(lpbih) (lpbih->biSize != sizeof(BITMAPCOREHEADER))
#define WIDTHBYTES(i)   ((((i)+31) >> 5) << 2)

BOOL Bitmap::DIBInfo (HANDLE hbi, LPBITMAPINFOHEADER lpbih)
{
    if (hbi){
      *lpbih = *(LPBITMAPINFOHEADER)hbi;

      /* fill in the default fields */
      if (NEW_DIB_FORMAT(lpbih)) {
        if (lpbih->biSizeImage == 0L)
          lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth*lpbih->biBitCount) * lpbih->biHeight;

        if (lpbih->biClrUsed == 0L)
          lpbih->biClrUsed = DIBNumColors (lpbih);
      }

      return TRUE;
    }
    return FALSE;
}

WORD Bitmap::DIBNumColors (LPVOID lpv)
{
    INT                 bits;
    LPBITMAPINFOHEADER  lpbih = (LPBITMAPINFOHEADER)lpv;
    LPBITMAPCOREHEADER  lpbch = (LPBITMAPCOREHEADER)lpv;

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (NEW_DIB_FORMAT(lpbih)) {
      if (lpbih->biClrUsed != 0)
        return (WORD)lpbih->biClrUsed;
      bits = lpbih->biBitCount;
    }
    else
      bits = lpbch->bcBitCount;

    if (bits > 8)
      return 0; /* Since biClrUsed is 0, we dont have a an optimal palette */
    else
      return (1 << bits);
}

Bitmap::Bitmap(HDC dc, HBITMAP bmp) {
	BITMAP bitmap;
	GetObject(bmp, sizeof(BITMAP), &bitmap);

    WORD cClrBits = (WORD)(bitmap.bmPlanes * bitmap.bmBitsPixel);
    if (cClrBits == 1)
    	cClrBits = 1;
    else if (cClrBits <= 4)
    	cClrBits = 4;
    else if (cClrBits <= 8)
    	cClrBits = 8;
    else if (cClrBits <= 16)
    	cClrBits = 16;
    else if (cClrBits <= 24)
    	cClrBits = 24;
    else cClrBits = 32;

    if (cClrBits < 24)
    	bmi_ = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits));
    else
    	bmi_ = (BITMAPINFO *) malloc(sizeof(BITMAPINFOHEADER));

    bmi_->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi_->bmiHeader.biWidth = bitmap.bmWidth;
    bmi_->bmiHeader.biHeight = bitmap.bmHeight;
    bmi_->bmiHeader.biPlanes = bitmap.bmPlanes;
    bmi_->bmiHeader.biBitCount = bitmap.bmBitsPixel;
    bmi_->bmiHeader.biCompression = BI_RGB;
    bmi_->bmiHeader.biSizeImage = 0;
    bmi_->bmiHeader.biXPelsPerMeter = 0;
    bmi_->bmiHeader.biYPelsPerMeter = 0;
    if (cClrBits < 24)
        bmi_->bmiHeader.biClrUsed = (1<<cClrBits);
    else
    	bmi_->bmiHeader.biClrUsed = 0;
    bmi_->bmiHeader.biClrImportant = 0;

    GetDIBits(dc, bmp, 0, (UINT)bitmap.bmHeight, NULL, bmi_, DIB_RGB_COLORS);

    DWORD bmpSize = bmi_->bmiHeader.biSizeImage;

    bits_ = malloc(bmpSize);

    GetDIBits(dc, bmp, 0, (UINT)bitmap.bmHeight, bits_, bmi_, DIB_RGB_COLORS);
}

Bitmap::~Bitmap() {
	free(bits_);
	free(bmi_);
}

Bitmap::Bitmap(const Bitmap &copyFrom) :
			bmi_{nullptr},
			bits_{nullptr} {
	DWORD bmpSize = ((copyFrom.bmi_->bmiHeader.biWidth * copyFrom.bmi_->bmiHeader.biBitCount + 31) / 32) * 4 * copyFrom.bmi_->bmiHeader.biHeight,
		  bmpInfoSize = copyFrom.bmi_->bmiHeader.biSize + sizeof(RGBQUAD) * copyFrom.bmi_->bmiHeader.biClrUsed;
	bmi_ = (BITMAPINFO *) malloc(bmpInfoSize);
	memcpy(bmi_, copyFrom.bmi_, bmpInfoSize);
	bits_ = malloc(bmpSize);
	memcpy(bits_, copyFrom.bits_, bmpSize);
}

HBITMAP Bitmap::CreateDDB(HDC dc) {
	return CreateDIBitmap(dc, &bmi_->bmiHeader, CBM_INIT,
			bits_, bmi_, DIB_RGB_COLORS);
}

const BITMAPINFO& Bitmap::getBitmapInfo() const {
	return *bmi_;
}

void * Bitmap::getBitmapBits() const {
	return bits_;
}
