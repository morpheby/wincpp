/*
 * TabButton.h
 *
 *  Created on: 01.12.2012
 *      Author: Morphe
 */

#ifndef TABBUTTON_H_
#define TABBUTTON_H_

#include "Window.h"
#include "TabCloseButton.h"

namespace Tabs {

class TabButton: public Window, public std::enable_shared_from_this<TabButton> {
public:
	TabButton();
	TabButton(const std::wstring &text, int x, int y, Window &parentWnd);
	void setName(const std::wstring &text);
	~TabButton();

	void setActive(bool active);
	void setOnCloseClick(WndEventBase *closeClicked);
protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList();
	void UpdateSizes();
private:
	RECT textRect_;
	bool active_, mouseHovering_, closeHover_;

	TabCloseButton close_;
	WndEventCaller closeClicked_;

	int onCloseClicked(Window &sender);
};

} /* namespace Tabs */
#endif /* TABBUTTON_H_ */
