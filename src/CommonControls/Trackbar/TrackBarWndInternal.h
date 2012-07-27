/*
 * TrackBarWnd.h
 *
 *  Created on: 7.5.2012
 *      Author: Morphe
 */

#ifndef TRACKBARWNDINT_H_
#define TRACKBARWNDINT_H_

#include <CommCtlWnd.h>
#include <Events.h>

enum class tbTicksPos {
	tbTicksTop = TBS_TOP,
	tbTicksBottom = TBS_BOTTOM,
	tbTicksLeft = TBS_LEFT,
	tbTicksRight = TBS_RIGHT,
	tbTicksBoth = TBS_BOTH
};

namespace TrackbarInternal {

class TrackBarWndInternal : public CommCtlWnd {
public:

	TrackBarWndInternal();
	TrackBarWndInternal(std::wstring name);
	TrackBarWndInternal(std::wstring name, int x, int y,
			int width, int height, bool horizontal, HWND parent);
	~TrackBarWndInternal();

	int getTrackbarPos();

	bool isHorizontal() const {
		return horizontal_;
	}

	void setRange(int low, int high);
	void setPageSize(int sz);
	void setPos(int pos);

	void setTicksPos(tbTicksPos pos);
	void setTicksFreq(int freq);
	void clearTicks();
	void setTick(int pos);
private:
	bool horizontal_;
};

}

#endif /* TRACKBARWNDINT_H_ */
