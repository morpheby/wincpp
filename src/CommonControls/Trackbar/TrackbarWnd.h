/*
 * TrackbarWnd.h
 *
 *  Created on: 12.5.2012
 *      Author: Morphe
 */

#ifndef TRACKBARWND_H_
#define TRACKBARWND_H_

#include "TrackBarWndInternal.h"

class TrackbarWnd : public Window {
public:
	TrackbarWnd();
	TrackbarWnd(std::wstring name, int x, int y,
			int width, int height, bool horizontal, HWND parent);
	~TrackbarWnd();

	Window * setLeftBuddy(Window *newBuddy);
	Window * setRightBuddy(Window *newBuddy);
	Window * setDownBuddy(Window *newBuddy);
	Window * setUpBuddy(Window *newBuddy);

	bool isHorizontal() const {
		return tb.isHorizontal();
	}

	void setOnPosChange(WndEventExtBase<int> *onPosChange) {
		this->onPosChange = onPosChange;
	}

	void setRange(int low, int high);
	void setPageSize(int sz);
	void setPos(int pos);

	void setTicksPos(tbTicksPos pos);
	void setTicksFreq(int freq);
	void clearTicks();
	void setTick(int pos);
protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void WMSize();
private:
	Window * setBuddy(Window *newBuddy, bool left);
	WndEventExtCaller<int> onPosChange;
	TrackbarInternal::TrackBarWndInternal tb;
};

#endif /* TRACKBARWND_H_ */
