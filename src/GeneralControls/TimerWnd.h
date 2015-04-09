/*
 * TimerWnd.h
 *
 *  Created on: 08.09.2013
 *      Author: morpheby
 */

#ifndef TIMERWND_H_
#define TIMERWND_H_

#include "Window.h"

class TimerWnd: public Window {
	WndEventCaller onTimeout_;
	int timeLapse_;
public:
	TimerWnd(int timeLapse);
	virtual ~TimerWnd();

	void setOnTimeout(WndEventBase *event);

	int getTimeLapse() const;

	void setTimeLapse(int timeLapse);

protected:
	virtual void timeout();

	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
};

#endif /* TIMERWND_H_ */
