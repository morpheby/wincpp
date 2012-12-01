/*
 * TabButton.cpp
 *
 *  Created on: 01.12.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TabButton.h"
#include <ThemedDrawer.h>

namespace Tabs {

TabButton::TabButton() :
	Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, nullptr, 0, 0),
	textRect_({0, 0, 0, 0}),
	active_{false},
	mouseHovering_{false} {

}

TabButton::TabButton(const std::wstring& text, int x, int y,
		Window& parentWnd) :
	Window(text, WS_CHILD | WS_VISIBLE, x, y, 0, 0, parentWnd, 0, 0),
	textRect_({0, 0, 0, 0}),
	active_{false},
	mouseHovering_{false} {
	setPaintCachingMode(false);
	CalcTxtRect();
	setSize(textRect_.right, textRect_.bottom);
	ImmediatelyUpdateWindow();
}

TabButton::~TabButton() {
}

LRESULT TabButton::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_MOUSEMOVE:
		mouseHovering_ = true;
		TrackMouseEvent(TME_LEAVE);
		ImmediatelyUpdateWindow();
		break;
	case WM_MOUSELEAVE:
		mouseHovering_ = false;
		ImmediatelyUpdateWindow();
		break;
	case WM_LBUTTONDOWN:
		break;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void TabButton::PaintWindow(Drawing::Drawer& drawer) {
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer)) {
		// Actually, TabItem doesn't have font, but do we care? ))
		tDrawer->setFontThemed(TABP_TABITEM, 0, TMT_FONT);
		RECT rect = getClientRect();
		if(!active_)
			tDrawer->drawThemedEdges(rect, TABP_TABITEM, TIS_NORMAL, BDR_RAISEDOUTER | BDR_SUNKENINNER, BF_BOTTOM);
	}
	SetBkMode(drawer.getDC(), TRANSPARENT);
	drawer.drawText(getName(), 0, textRect_);
	drawer.clearFont();
}

bool TabButton::WMEraseBackground(Drawing::Drawer& drawer) {
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer))
		tDrawer->setBackgroundThemed(TABP_TABITEM, mouseHovering_ ? TIS_HOT : (active_ ? TIS_SELECTED : TIS_NORMAL));
	return Window::WMEraseBackground(drawer);
}

std::wstring TabButton::GetThemeApplicableClassList() {
//	return L"Tab;" + Window::GetThemeApplicableClassList();
	return L"BrowserTab::Tab;" + Window::GetThemeApplicableClassList();
}

void TabButton::setName(const std::wstring& text) {
	Window::setName(text);
	CalcTxtRect();
	setSize(textRect_.right, textRect_.bottom);
	ImmediatelyUpdateWindow();
	::UpdateWindow(getParent());
}

void TabButton::CalcTxtRect() {
	memset(&textRect_, 0, sizeof(textRect_));
	textRect_.top += 5_scaled;
	textRect_.left += 5_scaled;
	std::shared_ptr<Drawing::Drawer> drawerPtr = getDrawer();
	if(std::shared_ptr<Drawing::ThemedDrawer> tDrawer = std::dynamic_pointer_cast<Drawing::ThemedDrawer>(drawerPtr))
		tDrawer->setFontThemed(TABP_TABITEM, TIS_NORMAL, TMT_FONT);
	drawerPtr->drawText(getName(), DT_CALCRECT, textRect_);
	drawerPtr->clearFont();
	textRect_.bottom += 5_scaled;
	textRect_.right += 5_scaled;
}

} /* namespace Tabs */
