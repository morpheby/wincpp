/*
 * ComboBoxWnd.cpp
 *
 *  Created on: 19.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "ComboBoxWnd.h"

using namespace std;
using namespace ComboBoxInternal;

ComboBoxWnd::ComboBoxWnd() : cb(0, 0, 0, 0, 0, *this) {
}

ComboBoxWnd::~ComboBoxWnd() {
}

ComboBoxWnd::ComboBoxWnd(int style, int x, int y, int width,
		int height, Window& parent) : Window(L"", WS_CHILD, x, y, width, height,
				parent, 0, 0), cb(style, 0, 0, width, height, *this) {
}

wstring ComboBoxWnd::getCurSel() const {
	return cb.getCurSel();
}

void ComboBoxWnd::selString(const wstring& str) {
	cb.selString(str);
	onSelChange(cb.getCurSelNum());
}

LRESULT ComboBoxWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_COMMAND:
		switch(HIWORD(wParam)) {
		case CBN_SELCHANGE:
			return onSelChange(cb.getCurSelNum());
		}
		break;
	case WM_SIZE:
		WMSize(MAKEPOINTS(lParam));
		break;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void ComboBoxWnd::openDropDown() {
	cb.openDropDown();
}

void ComboBoxWnd::closeDropDown() {
	cb.closeDropDown();
}

HWND ComboBoxWnd::getEditboxHandle() const {
	return FindWindowExW(cb, 0, WC_EDIT, 0);
}

int ComboBoxWnd::findString(const wstring& str, int startPos) const {
	return cb.findString(str, startPos);
}

wstring ComboBoxWnd::getItem(int i) const {
	return cb.getItem(i);
}

bool ComboBoxWnd::Autocomplete(EditboxWnd& eb,
		const wstring& inputText) {
	int i, j;
	unsigned int minNotEq = 0;
	if ((i = findString(inputText.c_str())) == CB_ERR)
		return false;

	wstring str1 = getItem(i);

	minNotEq = str1.size();
	j = i;
	while ((j = findString(inputText.c_str(), j)) > i) {
		// Partially auto-complete
		wstring str2 = getItem(j);
		unsigned int pos =
				mismatch(str1.begin(),
						str1.begin() + min(str1.size(), str2.size()),
						str2.begin()).first - str1.begin();
		if (pos < minNotEq)
			minNotEq = pos;
	}
	int sz = inputText.size();
	if(minNotEq == str1.size())
		selString(str1);
	else
		eb.setText(str1.substr(0, minNotEq));
	eb.setSelection(sz, minNotEq);
	return true;
}

void ComboBoxWnd::WMSize(POINTS size) {
	cb.setSize(size.x, size.y);
}

int ComboBoxWnd::getCurSelNum() const {
	return cb.getCurSelNum();
}

void ComboBoxWnd::setCurSelNum(int index) {
	cb.setCurSelNum(index);
}

bool ComboBoxWnd::isDropDownOpened() const {
	return cb.getDropDownState();
}
