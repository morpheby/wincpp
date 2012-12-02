/*
 * TabButton.cpp
 *
 *  Created on: 01.12.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TabCloseButton.h"
#include <ThemedDrawer.h>
#include <cassert>

namespace Tabs {

TabCloseButton::TabCloseButton() :
	Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, nullptr, 0, 0),
	state_{0},
	mousePressed_{false} {
}

TabCloseButton::TabCloseButton(int x, int y, int size,
		Window& parentWnd) :
	Window(L"", WS_CHILD | WS_VISIBLE, x, y, size, size, parentWnd, 0, 0),
	state_{0},
	mousePressed_{false} {
	setPaintCachingMode(false);
}

TabCloseButton::~TabCloseButton() {
}

LRESULT TabCloseButton::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	RECT tmpRect;
	switch(msg) {
	case WM_MOUSEMOVE:
		if(state_)
			break;
		state_ = 1;
		TrackMouseEvent(TME_LEAVE);
		ImmediatelyUpdateWindow();
		return 0;
	case WM_MOUSELEAVE:
		state_ = 0;
		mousePressed_ = false;
		tmpRect = getClientRect();
		::MapWindowPoints(getWindowHandle(), getParent(), (POINT *) &tmpRect, 2);
		SafeWindowFromHandle(getParent())->UpdateWindow(&tmpRect);
		::UpdateWindow(getParent());
		ImmediatelyUpdateWindow();
		return 0;
	case WM_LBUTTONDOWN:
		state_ = 2;
		mousePressed_ = true;
		ImmediatelyUpdateWindow();
		return 0;
	case WM_LBUTTONUP:
		state_ = 1;
		ImmediatelyUpdateWindow();
		if(mousePressed_)
			onClick_(*this);
		return 0;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void TabCloseButton::PaintWindow(Drawing::Drawer& drawer) {
	drawer.clearFont();
}

bool TabCloseButton::WMEraseBackground(Drawing::Drawer& drawer) {
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer))
		tDrawer->setBackgroundThemed(TTP_CLOSE, state_ == 2 ? TTCS_PRESSED : state_ == 1 ? TTCS_HOT : TTCS_NORMAL);
	return Window::WMEraseBackground(drawer);
}

std::wstring TabCloseButton::GetThemeApplicableClassList() {
	return L"TOOLTIP;" + Window::GetThemeApplicableClassList();
}

void TabCloseButton::setOnClick(WndEventBase* onClick) {
	onClick_ = onClick;
}

} /* namespace Tabs */
