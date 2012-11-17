/*
 * EditboxWnd.cpp
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
#include "EditboxWnd.h"

using namespace std;
using namespace EditboxInternal;

EditboxWndInternal& EditboxWnd::getEBInternal() const {
	return *eb;
}

EditboxWnd::EditboxWnd() :
		eText_(), eb(new EditboxWndInternal(0, 0, 0, 0, *this)) {
	InitEBInternal();
}

EditboxWnd::EditboxWnd(const wstring& initialText, int x, int y, int width, int height,
		HWND parent) :
		EditboxWnd(ES_AUTOHSCROLL, initialText, x, y, width, height, parent) {
	InitEBInternal();
}

EditboxWnd::EditboxWnd(int style, const wstring& initialText, int x, int y, int width,
		int height, HWND parent) :
	Window(L"", WS_CHILD, x, y, width, height, parent, 0, 0),
	 eText_(initialText), eb(new EditboxWndInternal(style, 0, 0, width, height, *this)) {
	InitEBInternal();
}

EditboxWnd::EditboxWnd(HWND convertFrom) :
		Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GetParent(convertFrom), 0, 0),
		eb(new EditboxWndInternal(convertFrom)) {
	InitEBInternal();
	// Inject itself in-between actual edit and it's parent
	setPosition(getEBInternal().getX(), getEBInternal().getY());
	setSize(getEBInternal().getWidth(), getEBInternal().getHeight());
	getEBInternal().setParent(*this);
	getEBInternal().setPosition(0, 0);
}

EditboxWnd::~EditboxWnd() {
}

void EditboxWnd::setText(const wstring& text) {
	text_ = text;
	getEBInternal().setText(text);
}

void EditboxWnd::setEmptyText(const wstring& eText) {
	getEBInternal().setEmptyText(eText);
}

LRESULT EditboxWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	int retval = 0;
	if(msg == WM_COMMAND)
		switch(HIWORD(wParam)) {
		case EN_CHANGE:
			// BUGFIX ComboBox behavior: passing this message to Window::WndProc makes
			// it to return zero, which leads to passing that to CB as previous
			// owner and disabling autocomplete behavior
			retval = 1;
			break;
		// EN_UPDATE is sent before window is redrawed, thus
		// changing text selection, or i.e. invoking autocomplete
		// is a bit smoother than with EN_CHANGE.
		case EN_UPDATE:
			if(getEBInternal().resetCharPlusFlag()) {
				wstring newTextVal = getEBInternal().getText();
				if(text_ == newTextVal)
					break;
				text_= newTextVal;
				retval = onTextInput_(*this, newTextVal);
			} else
				text_ = getEBInternal().getText();

			retval |= onTextChange_(*this, text_);
			break;
		}
	return retval ? : Window::WndProc(msg, wParam, lParam);
}

void EditboxWnd::setSelection(int start, int end) {
	getEBInternal().setSelection(start, end);
}

void EditboxWnd::WMSize() {
	if(getEBInternal().getWidth() != getWidth() ||
			getEBInternal().getHeight() != getHeight()) {
		getEBInternal().setSize(getWidth(), getHeight());
	}
}

bool EditboxWnd::WMSetFocus() {
	SetFocus(getEBInternal());
	return true;
}

bool EditboxWnd::WMKillFocus() {
	return true;
}

int EditboxWnd::OnSetFocusInternal(Window& sender) {
	return onSetFocus(*this); // pass event
}

int EditboxWnd::OnKillFocusInternal(Window& sender) {
	return onKillFocus(*this); // pass event
}

void EditboxWnd::InitEBInternal() {
	getEBInternal().setOnSetFocus(NewEvent(*this, &EditboxWnd::OnSetFocusInternal));
	getEBInternal().setOnKillFocus(NewEvent(*this, &EditboxWnd::OnKillFocusInternal));
	getEBInternal().setEmptyTextMode(true);
	getEBInternal().setEmptyText(eText_);
}

void EditboxWnd::setReadonly(bool ro) {
	Edit_SetReadOnly(getEBInternal(), ro);
}





