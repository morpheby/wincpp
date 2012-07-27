/*
 * LabelWnd.cpp
 *
 *  Created on: 17.05.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "LabelWnd.h"

using namespace std;

LabelWnd::LabelWnd() :
		Window(L"", 0, 0, 0, 0, 0, 0, 0, 0), txt(L""), txtRect({5, 5, 0, 0}), forcedWidth_(false) {
}

LabelWnd::LabelWnd(const wstring& text, int x, int y, HWND parentWnd) :
				Window(L"", WS_CHILD | WS_VISIBLE, x, y, 0, 0, parentWnd, 0, 0), txt(text),
				txtRect({5, 5, 0, 0}), forcedWidth_(false) {
	setText(text);
}

LabelWnd::~LabelWnd() {
}

void LabelWnd::setText(const wstring& text) {
	txt = text;
	CalcTxtRect();
	setSize(txtRect.right, txtRect.bottom);
	ImmediatelyUpdateWindow();
	::UpdateWindow(getParent());
}

wstring LabelWnd::getText() const {
	return txt;
}

void LabelWnd::PaintWindow(HDC hdc) {
	DrawTText(hdc, TEXT_LABEL, 0, txt, (isForcedWidth() ? DT_WORDBREAK : 0), txtRect);
}

bool LabelWnd::WMEraseBackground(HDC hdc) {
//	Hide();
//	InvalidateRect(getParent(), 0, TRUE);
//	::UpdateWindow(getParent());
//	DrawThemeParentBackground(getWindowHandle(), hdc, 0);
//	Show();
	Window::WMEraseBackground(hdc);
	return true;
}

wstring LabelWnd::GetThemeApplicableClassList() {
	return L"TEXTSTYLE;STATIC;" + Window::GetThemeApplicableClassList();
}

void LabelWnd::WMSize() {
	CalcTxtRect();
}

void LabelWnd::CalcTxtRect() {
	HDC dc = GetDC(getWindowHandle());
	if(isForcedWidth())
		GetClientRect(*this, &txtRect);
	else
		memset(&txtRect, 0, sizeof(txtRect));
	DrawTText(dc, TEXT_LABEL, 0, txt, DT_CALCRECT | (isForcedWidth() ? DT_WORDBREAK : 0), txtRect);
	ReleaseDC(getWindowHandle(), dc);
}



