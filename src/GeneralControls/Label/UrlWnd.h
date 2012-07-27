/*
 * URLWnd.h
 *
 *  Created on: 31.05.2012
 *      Author: Morphe
 */

#ifndef URLWND_H_
#define URLWND_H_

#include "LabelWnd.h"

class URLWnd: public LabelWnd {
public:
	URLWnd();
	URLWnd(const std::wstring &text, int x, int y, HWND parentWnd);
	~URLWnd();
protected:
	void PaintWindow(HDC hdc);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	bool visited;
};

#endif /* URLWND_H_ */
