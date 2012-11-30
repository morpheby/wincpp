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
	setStyleEx(WS_EX_CLIENTEDGE);
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
	delete buff;
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

void EditboxWndInternal::PaintWindow(HDC hdc) {
	if(getEmptyTextMode() && getTextLength() == 0) {
		RECT rect;
		Edit_GetRect(*this, &rect);
		LOGFONT font;
		HFONT prevFont;
		SetBkMode(hdc, TRANSPARENT);
		COLORREF prevColor = SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
		if(GetThemeFontInt(TEXT_BODYTEXT, 0, font))
			// GetThemeFont failed. Use fall-back font
			prevFont = (HFONT) SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
		else {
			font.lfItalic = true;
			prevFont = (HFONT) SelectObject(hdc, CreateFontIndirect(&font));
		}
		DrawTextW(hdc, eText_.c_str(), -1, &rect, DT_EDITCONTROL | DT_SINGLELINE);
		DeleteObject(SelectObject(hdc, prevFont));
		SetTextColor(hdc, prevColor);
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

void EditboxWndInternal::WMSize() {
	if(parentBack_)
		SafeWindowFromHandle(getParent())->setSize(getWidth(), getHeight());
}

int EditboxWndInternal::getTextLength() const {
	return Edit_GetTextLength(getWindowHandle());
}

void EditboxInternal::EditboxWndInternal::WMMove() {
	// Check if something moved us
	if(getX() != 0 || getY() != 0)
		// The position is locked to (0;0)
		setPosition(0, 0);
}
