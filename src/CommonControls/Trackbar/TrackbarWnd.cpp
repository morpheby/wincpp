/*
 * TrackbarWnd.cpp
 *
 *  Created on: 12.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TrackBarWndInternal.h"
#include "TrackbarWnd.h"

using namespace std;
using namespace TrackbarInternal;

TrackbarWnd::TrackbarWnd() : tb(new TrackBarWndInternal(L"", 0, 0, 0, 0, 0, *this)) {
}

TrackbarWnd::TrackbarWnd(wstring name, int x, int y, int width, int height,
		bool horizontal, HWND parent) :
		Window(name, WS_CHILD | WS_VISIBLE, x, y, width, height, parent, 0, 0),
		tb(new TrackBarWndInternal(name, 0, 0, width, height, horizontal, *this)) {
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
		onPosChange(*this, getTBInternal().getTrackbarPos());
		return 0;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void TrackbarWnd::WMSize() {
	getTBInternal().setSize(getWidth(), getHeight());
}

bool TrackbarWnd::isHorizontal() const {
	return getTBInternal().isHorizontal();
}

void TrackbarWnd::setRange(int low, int high) {
	getTBInternal().setRange(low, high);
}

void TrackbarWnd::setPageSize(int sz) {
	getTBInternal().setPageSize(sz);
}

void TrackbarWnd::setPos(int pos) {
	getTBInternal().setPos(pos);
	onPosChange(*this, getTBInternal().getTrackbarPos());
}

void TrackbarWnd::setTicksPos(tbTicksPos pos) {
	getTBInternal().setTicksPos(pos);
}

void TrackbarWnd::setTicksFreq(int freq) {
	getTBInternal().setTicksFreq(freq);
}

void TrackbarWnd::clearTicks() {
	getTBInternal().clearTicks();
}

void TrackbarWnd::setTick(int pos) {
	getTBInternal().setTick(pos);
}

TrackbarInternal::TrackBarWndInternal& TrackbarWnd::getTBInternal() const {
	return *tb;
}

Window* TrackbarWnd::setBuddy(Window* newBuddy, bool left) {
	return SafeWindowFromHandle((HWND)SendMessage(getTBInternal(),
			TBM_SETBUDDY, left,	(LPARAM) newBuddy->getWindowHandle()));
}


