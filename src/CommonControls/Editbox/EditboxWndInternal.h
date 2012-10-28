/*
 * EditboxWndInternal.h
 *
 *  Created on: 18.05.2012
 *      Author: Morphe
 */

#ifndef EDITBOXWNDINTERNAL_H_
#define EDITBOXWNDINTERNAL_H_

#include <CommCtlWnd.h>

namespace EditboxInternal {

class EditboxWndInternal : public CommCtlWnd {
public:
	EditboxWndInternal();
	EditboxWndInternal(int x, int y,
			int width, int height, HWND parent);
	EditboxWndInternal(int style, int x, int y,
			int width, int height, HWND parent);
	EditboxWndInternal(HWND convertFrom);
	~EditboxWndInternal();

	void setText(const std::wstring& text);
	std::wstring getText() const;

	void setEmptyText(const std::wstring& eText) {
		eText_ = eText;
		UpdateWindow();
	}

	void setSelection(int start, int end);

	void setEmptyTextMode(bool mode);
	bool getEmptyTextMode() const;

	bool resetCharPlusFlag() {
		return charPlus_ ? (charPlus_ = false), true : false;
	}

protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void PaintWindow(Drawing::Drawer &drawer) override;


private:
	bool etMode_, charPlus_, parentBack_;
	std::wstring eText_;
	int getTextLength() const;

	bool WMSetFocus();
	bool WMKillFocus();
	void WMChar(wchar_t ch);
	void WMMove(POINTS position);
	void WMSize(POINTS size);
};

}

#endif /* EDITBOXWNDINTERNAL_H_ */
