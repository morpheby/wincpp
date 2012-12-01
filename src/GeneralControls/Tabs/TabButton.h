/*
 * TabButton.h
 *
 *  Created on: 01.12.2012
 *      Author: Morphe
 */

#ifndef TABBUTTON_H_
#define TABBUTTON_H_

#include "Window.h"

namespace Tabs {

class TabButton: public Window {
public:
	TabButton();
	TabButton(const std::wstring &text, int x, int y, Window &parentWnd);
	void setName(const std::wstring &text);
	~TabButton();

	void setActive(bool active) {
		active_ = active;
	}
protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList();
	virtual void CalcTxtRect();
private:
	RECT textRect_;
	bool active_, mouseHovering_;
};

} /* namespace Tabs */
#endif /* TABBUTTON_H_ */
