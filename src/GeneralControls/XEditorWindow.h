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
#include <tuple>
#include <Cursors.h>

namespace XEditor {

struct XEditorOutputLineEventParams {
	Drawing::Drawer &partDrawer;
	const std::wstring &line;
	constexpr XEditorOutputLineEventParams(Drawing::Drawer &_partDrawer, const std::wstring &_line) : partDrawer(_partDrawer), line(_line) {}
};

class UndoMaintainer;

class XEditorWindow: public Window {
public:
	friend std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
	friend std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

	XEditorWindow();
	XEditorWindow(int x, int y, int width, int height, Window &parentWnd);
	~XEditorWindow();

	std::wstring getSelection() const;
	void removeSelection();
	void setSelection(int startLine, int startPosition, int line, int position);
	void invalidateScreen();
	void invalidateLine(int line);
	void invalidateCurrentLine();
	void invalidateLines(int start, int end);
	void repositionCaret();

	int getCurrentCharPosition() const;
	int getCurrentLinePosition() const;

	const std::wstring& getLine(int line) const;
	const std::wstring& getCurrentLine() const;
	const std::vector<std::wstring>& getLines() const;
	std::wstring getText(int startLine, int startCh, int endLine, int endCh) const;


	void setCurrentLinePosition(int line);
	void setCurrentCharPosition(int position);
	void setCurrentPosition(int line, int position);
	void setCurrentPosition(std::pair<int, int> pos) {
		setCurrentPosition(pos.first, pos.second);
	}
	void moveToLineEnd();
	void moveToNextLine();
	void moveToPrevChar();
	void moveToPrevLine();
	void moveToNextChar();
	void moveToLineBegin();

	std::wstring& insertLine(int after, const std::wstring &line);
	std::tuple<std::vector<std::wstring>::size_type, std::wstring::size_type>
	insertText(int line, int position, const std::wstring &text);
	void removeText(int startLine, int startPos, int endLine, int endPos);

	void setLine(int line, const std::wstring& str);
	void setCurrentLine(const std::wstring &line);

	int getStdCharHeight() const;

	std::pair<int, int> getSelectionStart() const;
	std::pair<int, int> getSelectionEnd() const;

	bool hasSelection() const;

	void selectDefFont();
	void selectFont(LOGFONT font);

	/// Identical to Ctrl+C
	void copySelection();

	/// Identical to Ctrl+X
	void cutSelection();

	/// Identical to Ctrl+V
	void pasteAtCursor();

	/// Identical to Ctrl+Z
	void undo();

	/// Identical to Ctrl+Y
	void redo();

protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	void PrePaintWindow(RECT &updateRect) override;
	void PaintWindow(Drawing::Drawer &drawer) override;
	void PostPaintWindow(RECT &updateRect) override;
	bool WMEraseBackground(Drawing::Drawer &drawer) override;
	std::wstring GetThemeApplicableClassList() override;

	void calcLineRect(const std::wstring &line, RECT &txtRect);
	virtual void outputLine(Drawing::Drawer &partDrawer, const std::wstring &line,
			RECT &txtRect, bool bSimulate = false, int selStart = -1, int selEnd = -1);
	virtual std::wstring processNewLine(int at, const std::wstring &line);

private:
	int stdCharHeight_{0};
	int currentLine_{0}, selStartLine_{-1}, screenStartY_{0};
	int currentChar_{0}, selStartChar_{-1}, screenStartX_{0};
//	bool selecting_{false};
	bool mouseHolding_{false};
	POINT mouseDownPoint_{0, 0};
	Cursor textCursor_{CursorFactory::GetCursor(0, IDC_IBEAM)},
			arrowCursor_{CursorFactory::GetCursor(0, IDC_ARROW)};
	LOGFONT textFont_;
	std::vector<std::wstring> lines_;
	WndEventExtCaller<XEditorOutputLineEventParams> onOutputLine_;
	std::unique_ptr<UndoMaintainer> undoMaintainer_;

	void WMSize(POINTS size);
	void WMKillFocus();
	void WMSetFocus();
	void charInput(wchar_t ch);
	void controlKey(wchar_t key);
	void WMKeyDown(char keyCode);
	void WMKeyUp(char keyCode);
	void mouseMsg(UINT msg, WPARAM wParam);
	void scrollMsg(UINT msg, WPARAM wParam);
	void mouseScroll(float scrollX, float scrollY);

	std::vector<std::wstring>& getLines();

	std::vector<std::wstring>::iterator getLineIt(int line);
//	void advanceLine(int offset);
	wchar_t removeCharAt(int line, int position);
	void removeLine(int line);
	void startSelection();
	void clearSelection();
	std::pair<int, int> getPositionAt(POINT pt);
	void updateScrollInfo();
	void scrollScreen();

	std::vector<std::wstring>::iterator int_insertLine(int at, const std::wstring &line);
};

std::ostream& operator<< (std::ostream& stream, XEditor::XEditorWindow& xeditor);
std::istream& operator>> (std::istream& stream, XEditor::XEditorWindow& xeditor);

} /* namespace XEditor */


#endif /* XEDITORWINDOW_H_ */
