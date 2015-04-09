/*
 * TimerWnd.cpp
 *
 *  Created on: 08.09.2013
 *      Author: morpheby
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TimerWnd.h"

#define TIMER_ID 1

TimerWnd::TimerWnd(int timeLapse) :
		timeLapse_(0) {
	setTimeLapse(timeLapse);
}

TimerWnd::~TimerWnd() {
	KillTimer(getWindowHandle(), TIMER_ID);
}

void TimerWnd::setOnTimeout(WndEventBase* event) {
	onTimeout_ = event;
}

int TimerWnd::getTimeLapse() const {
	return timeLapse_;
}

void TimerWnd::setTimeLapse(int timeLapse) {
	timeLapse_ = timeLapse;
	SetTimer(getWindowHandle(), TIMER_ID, timeLapse_, 0);
}

void TimerWnd::timeout() {
	onTimeout_(*this);
}

LRESULT TimerWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_TIMER && wParam == TIMER_ID)
		timeout();
	return Window::WndProc(msg, wParam, lParam);
}
