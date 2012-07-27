/*
 * DPIScaler.cpp
 *
 *  Created on: 18.05.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "DPIScaler.h"

#if HAVE_WINDOWS_H
#	include <windows.h>
#endif

bool DPIScaler::dpiReady = false;
int DPIScaler::dpiX_ = 0, DPIScaler::dpiY_ = 0;

int DPIScaler::ScaleX(int x) {
	InitDpiFactors();
	return MulDiv(x, dpiX_, 96);
}

int DPIScaler::ScaleY(int y) {
	InitDpiFactors();
	return MulDiv(y, dpiY_, 96);
}

int DPIScaler::ScaleWidth(int width) {
	return ScaleX(width);
}

int DPIScaler::ScaleHeight(int height) {
	return ScaleY(height);
}

int DPIScaler::UnscaleX(int x) {
	InitDpiFactors();
	return MulDiv(x, 96, dpiX_);
}

int DPIScaler::UnscaleY(int y) {
	InitDpiFactors();
	return MulDiv(y, 96, dpiY_);
}

void DPIScaler::InitDpiFactors() {
	if(dpiReady)
		return;

	HDC screen = GetDC(0);
	dpiX_ = GetDeviceCaps(screen, LOGPIXELSX);
    dpiY_ = GetDeviceCaps(screen, LOGPIXELSY);
    ReleaseDC(NULL, screen);
    dpiReady = true;
}

#if __cplusplus >= 201103L
unsigned long long operator"" _scaled (unsigned long long  x) {
	return DPIScaler::ScaleX(x);
}
#endif
