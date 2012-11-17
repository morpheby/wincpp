/*
 * EditboxWndInternal.cpp
 *
 *  Created on: 18.05.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_WINDOWSX_H
#include <windowsx.h>
#endif

#include "EditboxWndInternal.h"

using namespace std;
using namespace EditboxInternal;

EditboxWndInternal::EditboxWndInternal() : CommCtlWnd(VSCLASS_EDIT),
		etMode_(true), charPlus_(false), parentBack_(false) {
}

EditboxWndInternal::EditboxWndInternal(int style, int x, int y,
		int width, int height, HWND parent) : CommCtlWnd (L"",	style,
				parent, x, y, width, height, VSCLASS_EDIT), etMode_(true), charPlus_(false),
				parentBack_(false) {
	addStyleEx(WS_EX_CLIENTEDGE);
}

EditboxWndInternal::EditboxWndInternal(int x, int y, int width, int height,
		HWND parent) : EditboxWndInternal(0, x, y, width, height, parent) {
}

EditboxWndInternal::EditboxWndInternal(HWND convertFrom) :
	CommCtlWnd(convertFrom), etMode_(true), charPlus_(false), parentBack_(true) {

}

EditboxWndInternal::~EditboxWndInternal() {
}

void EditboxWndInternal::setText(const wstring& text) {
	Edit_SetText(getWindowHandle(), text.c_str());
}

wstring EditboxWndInternal::getText() const {
	size_t txtLength = getTextLength() + 1;
	wchar_t *buff = new wchar_t[txtLength];
	Edit_GetText(getWindowHandle(), buff, txtLength);
	wstring text(buff);
	delete[] buff;
	return text;
}

void EditboxWndInternal::setEmptyTextMode(bool mode) {
	etMode_ = mode;
	UpdateWindow();
}

bool EditboxWndInternal::getEmptyTextMode() const {
	return etMode_;
}

bool EditboxWndInternal::WMSetFocus() {
	setEmptyTextMode(false);
	return false;
}

bool EditboxWndInternal::WMKillFocus() {
	setEmptyTextMode(true);
	return false;
}

void EditboxWndInternal::PaintWindow(Drawing::Drawer &drawer) {
	if(getEmptyTextMode() && getTextLength() == 0) {
		RECT rect;
		Edit_GetRect(*this, &rect);
		LOGFONT font;
		SetBkMode(drawer.getDC(), TRANSPARENT);
		COLORREF prevColor = drawer.setTextColor(GetSysColor(COLOR_GRAYTEXT));
		drawer.setFontDefault();
		drawer.drawText(eText_, DT_EDITCONTROL | DT_SINGLELINE, rect);
		drawer.clearFont();
		drawer.setTextColor(prevColor);
	}
}

void EditboxWndInternal::setSelection(int start, int end) {
	Edit_SetSel(*this, start, end);
}

void EditboxWndInternal::WMChar(wchar_t ch) {
	if(isprint(ch) &&
			((getStyle() | ES_WANTRETURN) || (ch != '\r' && ch != '\n')))
		charPlus_ = true;
}

void EditboxWndInternal::WMSize(POINTS size) {
	if(parentBack_)
		SafeWindowFromHandle(getParent())->setSize(size.x, size.y);
}

LRESULT EditboxWndInternal::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_MOVE:
		WMMove(MAKEPOINTS(lParam));
		break;
	case WM_SIZE:
		WMSize(MAKEPOINTS(lParam));
		break;
	case WM_SETFOCUS:
		WMSetFocus();
		break;
	case WM_KILLFOCUS:
		WMKillFocus();
		break;
	case WM_CHAR:
		WMChar((wchar_t) wParam);
		break;
	default:
		break;
	}
	return CommCtlWnd::WndProc(msg, wParam, lParam);
}

int EditboxWndInternal::getTextLength() const {
	return Edit_GetTextLength(getWindowHandle());
}

void EditboxWndInternal::WMMove(POINTS pos) {
	// Check if something moved us
	if(pos.x != 0 || pos.y != 0)
		// The position is locked to (0;0)
		setPosition(0, 0);
}
