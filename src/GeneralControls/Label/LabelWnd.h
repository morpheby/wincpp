/*
 * LabelWnd.h
 *
 *  Created on: 17.05.2012
 *      Author: Morphe
 */

#ifndef LABELWND_H_
#define LABELWND_H_

#include <Window.h>

class LabelWnd : public Window{
public:
	LabelWnd();
	LabelWnd(const std::wstring &text, int x, int y, HWND parentWnd);
	~LabelWnd();
	void setText(const std::wstring &text);

	const std::wstring& getText() const;

	void setForcedWidth(bool forcedWidth) {
		forcedWidth_ = forcedWidth;
	}

protected:
	bool isForcedWidth() const {
		return forcedWidth_;
	}
	void PaintWindow(HDC hdc);
	bool WMEraseBackground(HDC hdc);
	void WMSize();
	std::wstring GetThemeApplicableClassList();
	void CalcTxtRect();
protected:
	std::wstring txt;
	RECT txtRect;
	bool forcedWidth_;
};




#endif /* LABELWND_H_ */
