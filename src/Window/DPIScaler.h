/*
 * DPIScaler.h
 *
 *  Created on: 18.05.2012
 *      Author: Morphe
 */

#ifndef DPISCALER_H_
#define DPISCALER_H_

class DPIScaler {
public:
	static int ScaleX(int x);
	static int ScaleY(int y);
	static int ScaleWidth(int width);
	static int ScaleHeight(int height);

	static int UnscaleX(int x);
	static int UnscaleY(int y);
private:
	static bool dpiReady;
	static int dpiX_, dpiY_;

	static void InitDpiFactors();
};

#if __cplusplus >= 201103L
unsigned long long operator"" _scaled (unsigned long long  x);
#endif

#endif /* DPISCALER_H_ */
