/*
 * XEditorWindow.h
 *
 *  Created on: 02.12.2012
 *      Author: Morphe
 */

#ifndef XEDITORWINDOW_H_
#define XEDITORWINDOW_H_

#include "Window.h"
#include <list>
#include <iostream>

namespace XEditor {

class XEditorWindow: public Window {
public:
	friend std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
	friend std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

	XEditorWindow();
	XEditorWindow(int x, int y, int width, int height, Window &parentWnd);
	~XEditorWindow();

	std::string getSelection() const;
	void setSelection(std::wstring);
	void invalidateScreen();
	void invalidateLine();
	void repositionCaret();
protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PrePaintWindow(RECT &updateRect) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	void PostPaintWindow(RECT &updateRect) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList() override;

	void calcTxtRect(const std::wstring &str, LOGFONT font, RECT &txtRect);
protected:
	int maxCharHeight_ {0};
	std::list<std::wstring>::iterator currentLine_, lastSelLine_;
	int currentChar_{0}, lastSelChar_{-1}, nCurrentLine_{0}, nLastSelLine{-1};
	LOGFONT textFont_;
	std::list<std::wstring> lines_;

	void WMSize(POINTS size);
	void WMKillFocus();
	void WMSetFocus();
	void WMChar(wchar_t ch);
	void WMKeyDown(char keyCode);
};

} /* namespace XEditor */

std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

#endif /* XEDITORWINDOW_H_ */
