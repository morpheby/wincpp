/*
 * Bitmap.h
 *
 *  Created on: 13.5.2012
 *      Author: Morphe
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <windows.h>

class Bitmap {
public:
	Bitmap(HDC dc, HBITMAP bmp);
	virtual ~Bitmap();

	HBITMAP CreateDDB(HDC dc);
private:
	BITMAPINFO bmi_;
	void *bits_;
};

#endif /* BITMAP_H_ */
