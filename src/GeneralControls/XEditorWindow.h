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
#include <vector>
#include <iostream>

namespace XEditor {

struct XEditorOutputLineEventParams {
	Drawing::Drawer &partDrawer;
	const std::wstring &line;
	constexpr XEditorOutputLineEventParams(Drawing::Drawer &_partDrawer, const std::wstring &_line) : partDrawer(_partDrawer), line(_line) {}
};

class XEditorWindow: public Window {
public:
	friend std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
	friend std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

	XEditorWindow();
	XEditorWindow(int x, int y, int width, int height, Window &parentWnd);
	~XEditorWindow();

	std::wstring getSelection() const;
	void replaceSelection(std::wstring selection);
	void invalidateScreen();
	void invalidateLine();
	void repositionCaret();

	int getCurrentCharPosition() const;
	int getCurrentLinePosition() const;

	const std::wstring& getLine(int line) const;
	const std::wstring& getCurrentLine() const;
	const std::vector<std::wstring>& getLines() const;


	void setCurrentLinePosition(int line);
	void setCurrentCharPosition(int position);
	void setCurrentPosition(int line, int position);
	void moveToLineEnd();
	void moveToNextLine();
	void moveToPrevChar();
	void moveToPrevLine();
	void moveToNextChar();
	void moveToLineBegin();

	std::wstring& insertLine(int after, const std::wstring &line);
	void insertText(int line, int position, const std::wstring &text);

	void setLine(int line, const std::wstring& str);
	void setCurrentLine(const std::wstring &line);

protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PrePaintWindow(RECT &updateRect) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	void PostPaintWindow(RECT &updateRect) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList() override;

	void calcTxtRect(const std::wstring &str, LOGFONT font, RECT &txtRect);
	virtual void outputLine(Drawing::Drawer &partDrawer, const std::wstring &line, RECT txtRect);

private:
	int maxCharHeight_ {0};
	int currentLine_{0}, lastSelLine_{-1}, screenStartLine_{0};
	int currentChar_{0}, lastSelChar_{-1}, screenStartChar_{0};
	int invalidatedLine_{-1};
	LOGFONT textFont_;
	std::vector<std::wstring> lines_;
	WndEventExtCaller<XEditorOutputLineEventParams> onOutputLine_;

	void WMSize(POINTS size);
	void WMKillFocus();
	void WMSetFocus();
	void WMChar(wchar_t ch);
	void WMKeyDown(char keyCode);

	std::vector<std::wstring>& getLines();

	std::vector<std::wstring>::iterator getLineIt(int line);
//	void advanceLine(int offset);
};

std::vector<std::wstring> split_lines(const std::wstring &text);
std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

} /* namespace XEditor */


#endif /* XEDITORWINDOW_H_ */
