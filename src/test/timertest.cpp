/*
 * timertest.cpp
 *
 *  Created on: 08.09.2013
 *      Author: Morphe
 */

#include <config.h>

#include <TimerWnd.h>
#include <memory>
#include <ctime>
#include <iostream>

class TimerCatcher {
	int counter_;
	std::unique_ptr<TimerWnd> timer_;
public:
	TimerCatcher() :
		counter_(0),
		timer_(new TimerWnd(100)) {
		timer_->setOnTimeout(NewEvent(*this, &TimerCatcher::onTimer));
	}
private:
	int onTimer(Window &sender) {
		if(++counter_ == 20)
			PostQuitMessage(0);
		return 0;
	}
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::unique_ptr<TimerCatcher> timerCatcher (new TimerCatcher());

	time_t startTime = time(0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if(time(0) - startTime > 3) {
			std::cerr <<  "Timeout not reached" << std::endl;
			return 1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}




