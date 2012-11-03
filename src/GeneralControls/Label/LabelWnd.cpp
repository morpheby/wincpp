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
#include <ThemedDrawer.h>

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

const wstring& LabelWnd::getText() const {
	return txt;
}

void LabelWnd::PaintWindow(Drawing::Drawer &drawer) {
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer))
		tDrawer->setFontThemed(TEXT_LABEL, 0);
	drawer.drawText(txt, (isForcedWidth() ? DT_WORDBREAK : 0), txtRect);
}

bool LabelWnd::WMEraseBackground(Drawing::Drawer &drawer) {
//	Hide();
//	InvalidateRect(getParent(), 0, TRUE);
//	::UpdateWindow(getParent());
//	DrawThemeParentBackground(getWindowHandle(), hdc, 0);
//	Show();
	Window::WMEraseBackground(drawer);
	return true;
}

wstring LabelWnd::GetThemeApplicableClassList() {
	return L"TEXTSTYLE;STATIC;" + Window::GetThemeApplicableClassList();
}

void LabelWnd::WMSize(POINTS size) {
	CalcTxtRect();
	if(txtRect.bottom != size.y || txtRect.right != size.x)
		setSize(txtRect.right, txtRect.bottom);
}

LRESULT LabelWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_SIZE)
		WMSize(MAKEPOINTS(lParam));
	return Window::WndProc(msg, wParam, lParam);
}

void LabelWnd::CalcTxtRect() {
	if(isForcedWidth())
		GetClientRect(*this, &txtRect);
	else
		memset(&txtRect, 0, sizeof(txtRect));
	std::shared_ptr<Drawing::Drawer> drawerPtr = getDrawer();
	if(shared_ptr<Drawing::ThemedDrawer> tDrawer = std::dynamic_pointer_cast<Drawing::ThemedDrawer>(drawerPtr))
		tDrawer->setFontThemed(TEXT_LABEL, 0);
	drawerPtr->drawText(txt, (isForcedWidth() ? DT_WORDBREAK | DT_CALCRECT : 0), txtRect);
}



