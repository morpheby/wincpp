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
	void PaintWindow(HDC hdc);
	bool WMSetFocus();
	bool WMKillFocus();
	void WMChar(wchar_t ch);
	void WMSize();
	void WMMove();
private:
	bool etMode_, charPlus_, parentBack_;
	std::wstring eText_;
	int getTextLength() const;
};

}

#endif /* EDITBOXWNDINTERNAL_H_ */
