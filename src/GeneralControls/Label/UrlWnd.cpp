/*
 * URLWnd.cpp
 *
 *  Created on: 31.05.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "URLWnd.h"

using namespace std;

URLWnd::URLWnd() : visited(false) {
}

URLWnd::URLWnd(const wstring& text, int x, int y, HWND parentWnd) : LabelWnd(text, x, y, parentWnd),
		visited(false) {
}

URLWnd::~URLWnd() {
}

void URLWnd::PaintWindow(HDC hdc) {
	LOGFONT font;
	HFONT prevFont;
	COLORREF prevColor = visited ?
			SetTextColor(hdc, RGB(25, 25, 255)) :
			SetTextColor(hdc, RGB(0, 0, 255));
	if(GetThemeFontInt(TEXT_HYPERLINKTEXT, TS_HYPERLINK_NORMAL, font))
		// GetThemeFont failed. Use fall-back font
		prevFont = (HFONT) SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	else {
		font.lfUnderline = true;
		prevFont = (HFONT) SelectObject(hdc, CreateFontIndirect(&font));
	}
	SetBkMode(hdc, TRANSPARENT);
	DrawTextW(hdc, txt.c_str(), -1, &txtRect, 0);
	DeleteObject(SelectObject(hdc, prevFont));
	SetTextColor(hdc, prevColor);
}

LRESULT URLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_LBUTTONUP) {
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		ShellExecuteW(*this, 0, txt.c_str(), 0, 0, SW_SHOWNORMAL);
		UpdateWindow();
	}

	return LabelWnd::WndProc(msg, wParam, lParam);
}



