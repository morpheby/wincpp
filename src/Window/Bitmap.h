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
	Bitmap(const Bitmap &copyFrom);
	virtual ~Bitmap();

	HBITMAP CreateDDB(HDC dc);

	const BITMAPINFO& getBitmapInfo() const;
	void *getBitmapBits() const;

private:
	BITMAPINFO *bmi_;
	void *bits_;

	static WORD DIBNumColors(LPVOID lpv);
	static BOOL DIBInfo (HANDLE hbi, LPBITMAPINFOHEADER lpbih);

};

#endif /* BITMAP_H_ */
