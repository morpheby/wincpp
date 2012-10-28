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

#include "EditboxWnd.h"

using namespace std;

EditboxWnd::EditboxWnd() :
		eText_(), eb(0, 0, 0, 0, *this) {
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
	 eText_(initialText), eb(style, 0, 0, width, height, *this) {
	InitEBInternal();
}

EditboxWnd::EditboxWnd(HWND convertFrom) :
		Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GetParent(convertFrom), 0, 0),
		eb(convertFrom) {
	InitEBInternal();
	// Inject itself in-between actual edit and it's parent
	setPosition(eb.getPosition().x, eb.getPosition().y);
	setSize(eb.getSize().cx, eb.getSize().cy);
	eb.setParent(*this);
	eb.setPosition(0, 0);
}

EditboxWnd::~EditboxWnd() {
}

void EditboxWnd::setText(const wstring& text) {
	text_ = text;
	eb.setText(text);
}

void EditboxWnd::setEmptyText(const wstring& eText) {
	eb.setEmptyText(eText);
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
			if(eb.resetCharPlusFlag()) {
				wstring newTextVal = eb.getText();
				if(text_ == newTextVal)
					break;
				text_= newTextVal;
				retval = onTextInput_(*this, newTextVal);
			} else
				text_ = eb.getText();

			retval |= onTextChange_(*this, text_);
			break;
		case WM_SIZE:
			WMSize();
			break;
		case WM_SETFOCUS:
			WMSetFocus();
			break;
		case WM_KILLFOCUS:
			WMKillFocus();
			break;
		}
	return retval ? : Window::WndProc(msg, wParam, lParam);
}

void EditboxWnd::setSelection(int start, int end) {
	eb.setSelection(start, end);
}

void EditboxWnd::WMSize() {
	if(eb.getSize().cx != getSize().cx || eb.getSize().cy != getSize().cy) {
		eb.setSize(getSize());
	}
}

int EditboxWnd::OnSetFocusInternal(Window& sender, WinMessage_t& msg) {
	return CallMsgProc(msg); // pass event
}

int EditboxWnd::OnKillFocusInternal(Window& sender, WinMessage_t& msg) {
	return CallMsgProc(msg); // pass event
}

void EditboxWnd::InitEBInternal() {
	eb.setProcessMessage(WM_SETFOCUS, NewEventExt(*this, &EditboxWnd::OnSetFocusInternal));
	eb.setProcessMessage(WM_KILLFOCUS, NewEventExt(*this, &EditboxWnd::OnKillFocusInternal));
	eb.setEmptyTextMode(true);
	eb.setEmptyText(eText_);
}

void EditboxWnd::setReadonly(bool ro) {
	Edit_SetReadOnly(eb, ro);
}





