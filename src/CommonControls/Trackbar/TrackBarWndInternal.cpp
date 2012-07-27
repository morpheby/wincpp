/*
 * TrackBarWnd.cpp
 *
 *  Created on: 7.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_WINDOWSX_H
#include <windowsx.h>
#endif

#include "TrackBarWndInternal.h"

using namespace std;
using namespace TrackbarInternal;

TrackBarWndInternal::TrackBarWndInternal() : CommCtlWnd(TRACKBAR_CLASS), horizontal_(true) {
}

TrackBarWndInternal::TrackBarWndInternal(wstring name) :
	CommCtlWnd(name, 0, 0, 0, 0, 0, 0, TRACKBAR_CLASS), horizontal_(true) {
}

TrackBarWndInternal::TrackBarWndInternal(wstring name, int x, int y, int width, int height,
		bool horizontal, HWND parent) :
		CommCtlWnd (name,
				(horizontal ? TBS_HORZ : TBS_VERT) | TBS_AUTOTICKS |
				TBS_TOOLTIPS | TBS_DOWNISLEFT,
				parent, x, y, width, height, TRACKBAR_CLASS),
				horizontal_(horizontal) {
}

TrackBarWndInternal::~TrackBarWndInternal() {
}

int TrackBarWndInternal::getTrackbarPos() {
	return (int)  SendMessage(getWindowHandle(), TBM_GETPOS, 0, 0);
}

void TrackBarWndInternal::setRange(int low, int high) {
	SendMessage(getWindowHandle(), TBM_SETRANGEMIN, 0, (LPARAM) low);
	SendMessage(getWindowHandle(), TBM_SETRANGEMAX, 1, (LPARAM) high);
}

void TrackBarWndInternal::setPageSize(int sz) {
	SendMessage(getWindowHandle(), TBM_SETPAGESIZE, 0, (LPARAM) sz);
}

void TrackBarWndInternal::setPos(int pos) {
	SendMessage(getWindowHandle(), TBM_SETPOS, 1, (LPARAM) pos);
}

void TrackBarWndInternal::setTicksPos(tbTicksPos pos) {
	clearStyle(TBS_TOP | TBS_BOTTOM | TBS_LEFT | TBS_RIGHT);
	setStyle((int)pos);
}

void TrackBarWndInternal::setTicksFreq(int freq) {
	SendMessage(getWindowHandle(), TBM_SETTICFREQ, freq, 0);
}

void TrackBarWndInternal::clearTicks() {
	SendMessage(getWindowHandle(), TBM_CLEARTICS, 1, 0);
}

void TrackBarWndInternal::setTick(int pos) {
	SendMessage(getWindowHandle(), TBM_SETTIC, 0, (LPARAM) pos);
}




