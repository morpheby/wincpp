/*
 * RenameWnd.cpp
 *
 *  Created on: 01.06.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "RenameWnd.h"

using namespace std;

RenameWnd::RenameWnd(const wstring& str, int x, int y, int width, int height,
		Window& parent) :
		Window(L"Rename", WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_DLGFRAME, x, y, width, 80_scaled, parent, 0, 0),
		str_(str),
		edit(L"New name...", 0, 0, getSize().cx, getSize().cy - 25_scaled, *this),
		okBtn(ButtonWnd::DefPushButton, L"OK", 0, edit.getSize().cy, getSize().cx, 25_scaled, *this) {
	edit.setText(str);
	edit.Show();
	okBtn.setOnClick(NewEvent(*this, &RenameWnd::OkClicked));
}

RenameWnd::~RenameWnd() {
	str_ = edit.getText();
}

bool RenameWnd::WMClose() {
	str_ = edit.getText();
	return false;
}

void RenameWnd::WMDestroy() {
	delete this;
}


int RenameWnd::OkClicked(Window& wnd) {
	str_ = edit.getText();
	onClose(*this);
	DestroyWindow(*this);
	return 0;
}


