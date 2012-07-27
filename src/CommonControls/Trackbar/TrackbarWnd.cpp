/*
 * TrackbarWnd.cpp
 *
 *  Created on: 12.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TrackbarWnd.h"

using namespace std;

TrackbarWnd::TrackbarWnd() : tb(L"", 0, 0, 0, 0, 0, *this) {
}

TrackbarWnd::TrackbarWnd(wstring name, int x, int y, int width, int height,
		bool horizontal, HWND parent) :
		Window(name, WS_CHILD | WS_VISIBLE, x, y, width, height, parent, 0, 0),
		tb(name, 0, 0, width, height, horizontal, *this) {
}

TrackbarWnd::~TrackbarWnd() {
}

Window* TrackbarWnd::setLeftBuddy(Window* newBuddy) {
	return setBuddy(newBuddy, true);
}

Window* TrackbarWnd::setRightBuddy(Window* newBuddy) {
	return setBuddy(newBuddy, false);
}

Window* TrackbarWnd::setDownBuddy(Window* newBuddy) {
	return setBuddy(newBuddy, false);
}

Window* TrackbarWnd::setUpBuddy(Window* newBuddy) {
	return setBuddy(newBuddy, true);
}

LRESULT TrackbarWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_HSCROLL || msg == WM_VSCROLL) {
		onPosChange(*this, tb.getTrackbarPos());
		return 0;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void TrackbarWnd::WMSize() {
	tb.setSize(getWidth(), getHeight());
}

void TrackbarWnd::setRange(int low, int high) {
	tb.setRange(low, high);
}

void TrackbarWnd::setPageSize(int sz) {
	tb.setPageSize(sz);
}

void TrackbarWnd::setPos(int pos) {
	tb.setPos(pos);
	onPosChange(*this, tb.getTrackbarPos());
}

void TrackbarWnd::setTicksPos(tbTicksPos pos) {
	tb.setTicksPos(pos);
}

void TrackbarWnd::setTicksFreq(int freq) {
	tb.setTicksFreq(freq);
}

void TrackbarWnd::clearTicks() {
	tb.clearTicks();
}

void TrackbarWnd::setTick(int pos) {
	tb.setTick(pos);
}

Window* TrackbarWnd::setBuddy(Window* newBuddy, bool left) {
	return SafeWindowFromHandle((HWND)SendMessage(tb.getWindowHandle(),
			TBM_SETBUDDY, left,	(LPARAM) newBuddy->getWindowHandle()));
}


