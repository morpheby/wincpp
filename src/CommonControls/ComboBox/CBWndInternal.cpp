/*
 * CBWndInternal.cpp
 *
 *  Created on: 20.5.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_WINDOWSX_H
#include <windowsx.h>
#endif

#include "CBWndInternal.h"

using namespace std;

namespace ComboBoxInternal {

CBWndInternal::CBWndInternal() : CommCtlWnd(VSCLASS_COMBOBOX) {
}

CBWndInternal::CBWndInternal(int style, int x, int y, int width, int height,
		Window& parent) :
		CommCtlWnd(L"", style,
				parent, x, y, width, height, VSCLASS_COMBOBOX) {
}

CBWndInternal::~CBWndInternal() {
}

void CBWndInternal::addString(const wstring& str) {
	ComboBox_AddString(*this, str.c_str());
}

void CBWndInternal::selString(const wstring& str) {
	setCurSelNum(findString(str));
}

int CBWndInternal::getCurSelNum() const {
	return ComboBox_GetCurSel(*this);
}

wstring CBWndInternal::getItem(int i) const {
	if(i == CB_ERR) return L"";
	size_t txtLength = getStrSize(i) + 1;
	wchar_t* buff = new wchar_t[txtLength];
	ComboBox_GetLBText(*this, i, buff);
	wstring text(buff);
	delete buff;
	return text;
}

wstring CBWndInternal::getCurSel() const {
	return getItem(getCurSelNum());
}

void CBWndInternal::clearList() {
	ComboBox_ResetContent(*this);
}

int CBWndInternal::findString(const wstring& str, int startPos) const {
	return ComboBox_FindString(*this, startPos, str.c_str());
}

bool CBWndInternal::getDropDownState() const {
	return ComboBox_GetDroppedState(*this);
}

int CBWndInternal::getStrSize(int index) const {
	return ComboBox_GetLBTextLen(*this, index);
}

void CBWndInternal::openDropDown() {
	if(!getDropDownState()) {
		// BUGFIX: ComboBoxProc calls SetCapture to capture mouse
		// window. If the mouse cursor was hidden previously by
		// SetCursor(0), it will not reappear on mouse move. Thus,
		// we shall show it ourselves
		SetCursor(LoadCursor(0, IDC_ARROW));
		SendMessage(*this, CB_SHOWDROPDOWN, 1, 0);
	}
}

void CBWndInternal::closeDropDown() {
	if(getDropDownState())
		SendMessage(*this, CB_SHOWDROPDOWN, 0, 0);
}

void CBWndInternal::setCurSelNum(int index) {
	ComboBox_SetCurSel(*this, index);
}

/* namespace ComboBoxInternal */

}

