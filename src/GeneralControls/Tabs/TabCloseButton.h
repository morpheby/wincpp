/*
 * TabButton.h
 *
 *  Created on: 01.12.2012
 *      Author: Morphe
 */

#ifndef TABCLOSEBUTTON_H_
#define TABCLOSEBUTTON_H_

#include "Window.h"

namespace Tabs {

class TabCloseButton : public Window {
public:
	TabCloseButton();
	TabCloseButton(int x, int y, int size, Window &parentWnd);
	~TabCloseButton();

	short int getState() const {
		return state_;
	}

	void setOnClick(WndEventBase *onClick);

protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList();

private:
	short int state_;
	bool mousePressed_;
	WndEventCaller onClick_;
};

} /* namespace Tabs */
#endif /* TABCLOSEBUTTON_H_ */
