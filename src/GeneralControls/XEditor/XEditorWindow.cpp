/*
 * XEditorWindow.cpp
 *
 *  Created on: 02.12.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "XEditorWindow.h"
#include <sstream>
#include <string.h>

#define NEWLINE L"\r\n"

namespace XEditor {

std::vector<std::wstring> split_lines(const std::wstring &text);
std::wstring PasteFromClipboard(Window &wnd);
void CopyToClipboard(Window &wnd, const std::wstring &text);
std::wstring StrTrimRight(const std::wstring& str);
bool Inbetween(int a, int b, int x);

XEditorWindow::XEditorWindow() :
	Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, 0, 0, 0){
}

XEditorWindow::XEditorWindow(int x, int y, int width, int height,
		Window& parentWnd) :
	Window(L"", WS_CHILD | WS_VISIBLE, x, y, width, height, parentWnd, 0, 0) {
//	textFont_ = Drawing::Drawer::getLogFont((HFONT)GetStockObject(SYSTEM_FIXED_FONT));
	selectDefFont();
	lines_.push_back(L"");
	RECT t {0, 0, 0, 0};
	calcLineRect(L"A", t); // since we are here, we really suppose to call XEditorWindow::calcLineRect - NOT it's derivatives
	stdCharHeight_ = t.bottom;
}

XEditorWindow::~XEditorWindow() {
}

LRESULT XEditorWindow::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_SIZE:
		WMSize(MAKEPOINTS(lParam));
		break;
	case WM_SETFOCUS:
		WMSetFocus();
		return 0;
	case WM_KILLFOCUS:
		WMKillFocus();
		return 0;
	case WM_CHAR:
		if(!(GetKeyState(VK_CONTROL) & 0x80))
			charInput((wchar_t)wParam);
		break;
	case WM_KEYDOWN:
		WMKeyDown((char) wParam);
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		mouseMsg(msg);
		break;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void XEditorWindow::PaintWindow(Drawing::Drawer& drawer) {
	POINT org;
	drawer.setFont(CreateFontIndirect(&textFont_));
	SetBkMode(drawer.getDC(), TRANSPARENT);
	GetWindowOrgEx(drawer.getDC(), &org);
//	HRGN oldClipRgn = CreateRectRgn(0, 0, 0, 0);
//	GetClipRgn(drawer.getDC(), oldClipRgn);
	int selectionLineStart = getSelectionStart().first,
		selectionLineEnd = getSelectionEnd().first,
		selectionStart = getSelectionStart().second,
		selectionEnd = getSelectionEnd().second;

	int top = - screenStartY_;
	bool bVisibleSectionReached = false;
	for(int i = 0; i < getLines().size() && top < getSizeY(); ++i) {
		if(getLine(i).empty()) {
			top += stdCharHeight_;
			continue;
		}

		RECT txtRect {0, 0, 0, 0};
		SetWindowOrgEx(drawer.getDC(), org.x - screenStartX_, org.y - top, 0);
		if(!bVisibleSectionReached) {
			outputLine(drawer, getLine(i), txtRect, true);
			if(top + txtRect.bottom > 0)
				bVisibleSectionReached = true;
		}
		if(bVisibleSectionReached)
			outputLine(drawer, getLine(i), txtRect, false,
					!hasSelection() ?
							-1 :
					i == selectionLineStart ?
							selectionStart :
					(selectionLineStart < i && i <= selectionLineEnd) ?
							0 : -1,
					i == selectionLineEnd ?
							selectionEnd : -1);
//		HRGN newClipRgn = CreateRectRgnIndirect(&txtRect);
//		SelectClipRgn(drawer.getDC(), newClipRgn);
//		DeleteObject(newClipRgn);

		top += txtRect.bottom - txtRect.top;
	}
	SetWindowOrgEx(drawer.getDC(), org.x, org.y, 0);
//	SelectClipRgn(drawer.getDC(), oldClipRgn);
//	DeleteObject(oldClipRgn);
	drawer.clearFont();
}

bool XEditorWindow::WMEraseBackground(Drawing::Drawer& drawer) {
	Window::WMEraseBackground(drawer);
	return true;
}

std::wstring XEditorWindow::GetThemeApplicableClassList() {
	return L"TEXTSTYLE;" + Window::GetThemeApplicableClassList();
}

void XEditorWindow::calcLineRect(const std::wstring &line, RECT &txtRect) {
	std::shared_ptr<Drawing::Drawer> drawerPtr = getDrawer();
	drawerPtr->setFont(CreateFontIndirect(&textFont_));
	outputLine(*drawerPtr, line, txtRect, true);
	drawerPtr->clearFont();
	if(txtRect.bottom - txtRect.top < stdCharHeight_)
		txtRect.bottom = txtRect.top + stdCharHeight_;
}

void XEditorWindow::WMSize(POINTS size) {
}

void XEditorWindow::WMKillFocus() {
    HideCaret(*this);
    DestroyCaret();
}

void XEditorWindow::WMSetFocus() {
    ::CreateCaret(*this, 0, 0, stdCharHeight_);
    repositionCaret();
    ::ShowCaret(*this);
}

std::wstring XEditorWindow::getSelection() const {
	if(!hasSelection())
		return L""; // no selection


	if(getSelectionStart().first == getSelectionEnd().first)
		return getLine(getSelectionStart().first).substr(getSelectionStart().second, getSelectionEnd().second - getSelectionStart().second);

	std::wstring selection;
	selection += getLine(getSelectionStart().first).substr(getSelectionStart().second) + NEWLINE;
	for(int i = getSelectionStart().first + 1; i < getSelectionEnd().first; ++i)
		selection += getLine(i) + NEWLINE;
	selection += getLine(getSelectionEnd().first).substr(0, getSelectionEnd().second);

	return selection;
}

void XEditorWindow::removeSelection() {
	int selectionStartLine = getSelectionStart().first,
		selectionStartChar = getSelectionStart().second,
		selectionEndLine = getSelectionEnd().first,
		selectionEndChar = getSelectionEnd().second;

	setCurrentPosition(selectionStartLine, selectionStartChar);

	setLine(selectionStartLine, getLine(selectionStartLine).substr(0, selectionStartChar) +
			getLine(selectionEndLine).substr(selectionEndChar));

	if(getSelectionStart().first != getSelectionEnd().first) {
		getLines().erase(getLineIt(selectionStartLine + 1), getLineIt(selectionEndLine));
		invalidateLines(selectionStartLine, getLines().size());
	} else
		invalidateLine(selectionStartLine);

}

void XEditorWindow::invalidateScreen() {
	UpdateWindow();
}

void XEditorWindow::invalidateLines(int start, int end) {
	RECT updateRect {0, 0, getSizeX(), 0};
	int top = - screenStartY_;

	if(start < 0)
		start = 0;
	if(end > getLines().size())
		end = getLines().size();

	for(int i = 0; i < end && top < getSizeY(); ++i) {
		RECT txtRect {0, 0, 0, 0};
		if(getLine(i).empty())
			txtRect.bottom = stdCharHeight_;
		else
			calcLineRect(getLine(i), txtRect);
		if(i == start)
			updateRect.top = top;

		top += txtRect.bottom - txtRect.top;
	}
	if(end == getLines().size())
		updateRect.bottom = getSizeY();
	else
		updateRect.bottom = top;

	UpdateWindow(&updateRect);
}

void XEditorWindow::PrePaintWindow(RECT& updateRect) {
	HideCaret(*this);
}

void XEditorWindow::PostPaintWindow(RECT& updateRect) {
	ShowCaret(*this);
}

void XEditorWindow::repositionCaret() {
    RECT txtRect {0, 0, 0, 0};
    calcLineRect(getCurrentLine().substr(0, currentChar_), txtRect);
    ::SetCaretPos(txtRect.right, getCurrentLinePosition() * stdCharHeight_);

}

void XEditorWindow::invalidateCurrentLine() {
	invalidateLine(getCurrentLinePosition());
}

void XEditorWindow::removeCharAt(int line, int position) {
	if(position == -1) {
		// concat with prev line
		if(line == 0)
			return;
		setLine(line - 1, getLine(line - 1) + getLine(line));
		invalidateLine(line - 1);
		removeLine(line);
	} else if(position == getLine(line).size()) {
		// concat with next line
		if(line == getLines().size() - 1)
			return;
		setLine(line, getLine(line) + getLine(line + 1));
		invalidateLine(line);
		removeLine(line + 1);
	} else {
		getLineIt(line)->erase(position, 1);
		invalidateLine(line);
	}

}

void XEditorWindow::charInput(wchar_t ch) {
	switch (ch)
	{
	case 0x0A:  // linefeed
	case 0x1B:  // escape
		MessageBeep((UINT) -1);
		break;

	case 0x08:  // backspace
		if(!hasSelection()) {
			int line = getCurrentLinePosition(),
				pos = getCurrentCharPosition();
			moveToPrevChar();
			removeCharAt(line, pos - 1);
		} else
			removeSelection();
		break;

	case 0x0D:  // carriage return
		removeSelection();
//		insertLine(currentLine_ + 1, L"");
		insertText(getCurrentLinePosition(), getCurrentCharPosition(), L"\n");
		moveToNextLine();
		break;

	default:    // displayable character
		getLineIt(getCurrentLinePosition())->insert(getLineIt(getCurrentLinePosition())->begin() + getCurrentCharPosition(), ch);
		moveToNextChar();
		repositionCaret();
		invalidateCurrentLine();
		break;
	}
}

void XEditorWindow::moveToLineEnd() {
	setCurrentCharPosition(getCurrentLine().size());
}

void XEditorWindow::moveToLineBegin() {
	setCurrentCharPosition(0);
}

void XEditorWindow::moveToNextLine() {
	setCurrentLinePosition(getCurrentLinePosition() + 1);
}

void XEditorWindow::moveToPrevLine() {
	setCurrentLinePosition(getCurrentLinePosition() - 1);
}

void XEditorWindow::moveToPrevChar() {
	if(getCurrentCharPosition() == 0 &&
			getCurrentLinePosition() != 0) {
		moveToPrevLine();
		moveToLineEnd();
	} else
		setCurrentCharPosition(getCurrentCharPosition() - 1);
}

void XEditorWindow::moveToNextChar() {
	if(getCurrentCharPosition() == getCurrentLine().size() &&
			getCurrentLinePosition() != getLines().size() - 1)
		moveToNextLine();
	else
		setCurrentCharPosition(getCurrentCharPosition() + 1);
}

void XEditorWindow::setCurrentLinePosition(int line) {
	if(hasSelection())
		invalidateLines(std::min(line, currentLine_), std::max(line, currentLine_) + 1);
	if(line < 0)
		currentLine_ = 0;
	else if(line >= getLines().size())
		currentLine_ = getLines().size() - 1;
	else
		currentLine_ = line;
	setCurrentCharPosition(0);
}

void XEditorWindow::setCurrentCharPosition(int position) {
	if(hasSelection())
		invalidateLine(currentLine_);
	if(position < 0)
		currentChar_ = 0;
	else if(position > getCurrentLine().size())
		currentChar_ = getCurrentLine().size();
	else
		currentChar_ = position;
	repositionCaret();
}

void XEditorWindow::setCurrentPosition(int line, int position) {
	setCurrentLinePosition(line);
	setCurrentCharPosition(position);
}

std::wstring& XEditorWindow::insertLine(int at, const std::wstring &line) {
	auto res = getLines().insert(getLineIt(at), line);

	if(currentLine_ > at)
		++currentLine_;

	repositionCaret();
	invalidateLines(at, getLines().size());
	return *res;
}

std::tuple<std::vector<std::wstring>::size_type, std::wstring::size_type>
XEditorWindow::insertText(int line, int position,
		const std::wstring& text) {
	std::vector<std::wstring> lines = split_lines(text);
	if(lines.empty())
		return std::make_tuple(line, position);
	std::wstring t = getLine(line).substr(0, position), rem = getLine(line).substr(position);

	t += lines.front();
	setLine(line, t);

	getLines().insert(getLineIt(line+1), lines.begin() + 1, lines.end());
	*getLineIt(line + lines.size() - 1) += rem;

	if(currentLine_ > line)
		currentLine_ += lines.size() - 1;
	else if(currentLine_ == line && currentChar_ > position) {
		currentLine_ += lines.size() - 1;
		currentChar_ += lines.back().size() - t.size();
	}

	repositionCaret();
	invalidateLines(line, getLines().size());

	if(lines.size() == 1)
		return std::make_tuple(line, position + lines.back().size());
	else
		return std::make_tuple(line + lines.size() - 1, lines.back().size());
}

//void XEditorWindow::advanceLine(int diff) {
//	if(diff > 0)	// for lists
//		for(int i = 0; i < diff; ++i)
//			++currentLine_;
//	else
//		for(int i = 0; i > diff; --i)
//			--currentLine_;
//	currentLine_ += diff;
//}

void XEditorWindow::WMKeyDown(char keyCode) {
	int lastPosition;

	if(GetKeyState(VK_CONTROL) & 0x80)
		controlKey(keyCode);

	bool shiftDown = GetKeyState(VK_SHIFT) & 0x80;

	switch(keyCode) {
	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		if(shiftDown && !selecting_)
			startSelection();
		else if(!shiftDown && selecting_)
			clearSelection();
		break;
	default:
		break;
	}

	switch(keyCode) {
	case VK_UP:
		lastPosition = getCurrentCharPosition();
		moveToPrevLine();
		setCurrentCharPosition(lastPosition);
		break;
	case VK_DOWN:
		lastPosition = getCurrentCharPosition();
		moveToNextLine();
		setCurrentCharPosition(lastPosition);
		break;
	case VK_LEFT:
		moveToPrevChar();
		break;
	case VK_RIGHT:
		moveToNextChar();
		break;
	case VK_HOME:
		moveToLineBegin();
		break;
	case VK_END:
		moveToLineEnd();
		break;
	case VK_DELETE:
		if(!hasSelection())
			removeCharAt(getCurrentLinePosition(), getCurrentCharPosition());
		else
			removeSelection();
		break;
	default:
		break;
	}
}

void XEditorWindow::WMKeyUp(char keyCode) {
}

std::ostream& operator <<(std::ostream& stream,
		XEditor::XEditorWindow& xeditor) {
}

std::istream& operator >>(std::istream& stream,
		XEditor::XEditorWindow& xeditor) {
}

int XEditorWindow::getCurrentCharPosition() const {
	return currentChar_;
}

int XEditorWindow::getCurrentLinePosition() const {
	return currentLine_;
}

const std::wstring& XEditorWindow::getCurrentLine() const {
	return getLine(currentLine_);
}

const std::vector<std::wstring>& XEditorWindow::getLines() const {
	return lines_;
}

std::vector<std::wstring>::iterator XEditorWindow::getLineIt(int line) {
	return getLines().begin() + line;
}

const std::wstring& XEditorWindow::getLine(int line) const{
	return getLines()[line];
}

void XEditorWindow::setCurrentLine(const std::wstring& line) {
	setLine(getCurrentLinePosition(), line);
}

void XEditorWindow::outputLine(Drawing::Drawer& partDrawer,
		const std::wstring& line, RECT &txtRect, bool bSimulate, int selStart, int selEnd) {

	partDrawer.drawText(line, DT_CALCRECT, txtRect);

	if(selStart != -1) {
		RECT leftRect {0, 0, 0, 0}, fillRect {0, 0, 0, 0}, rightRect {0, 0, 0, 0};
		partDrawer.drawText(line.substr(0, selStart), DT_CALCRECT, leftRect);
		fillRect.left = leftRect.right;
		if(selEnd == -1) {
			partDrawer.drawText(line.substr(selStart), DT_CALCRECT, fillRect);
			fillRect.right = getSizeX();
		} else {
			partDrawer.drawText(line.substr(selStart, selEnd - selStart), DT_CALCRECT, fillRect);
			rightRect.left = fillRect.right;
			partDrawer.drawText(line.substr(selEnd), DT_CALCRECT, rightRect);
		}
		if(fillRect.bottom - fillRect.top < stdCharHeight_)
			fillRect.bottom = fillRect.top + stdCharHeight_;

		HBRUSH br = CreateSolidBrush(RGB(0, 0, 200));
		partDrawer.fillRect(fillRect, br);
		DeleteObject(br);

		if(!bSimulate) {
			partDrawer.drawText(line.substr(0, selStart), 0, leftRect);
			partDrawer.setTextColor(RGB(0xFF, 0xFF, 0xFF));
			if(selEnd != -1) {
				partDrawer.drawText(line.substr(selStart, selEnd - selStart), 0, fillRect);
				partDrawer.setTextColor(RGB(0, 0, 0));
				partDrawer.drawText(line.substr(selEnd), 0, rightRect);
			} else {
				partDrawer.drawText(line.substr(selStart), 0, fillRect);
				partDrawer.setTextColor(RGB(0, 0, 0));
			}
		}
	} else if(!bSimulate)
		partDrawer.drawText(line, 0, txtRect);
}

void XEditorWindow::copySelection() {
	if (hasSelection())
		CopyToClipboard(*this, getSelection());
}

void XEditorWindow::pasteAtCursor() {
	std::wstring buf;
	buf = PasteFromClipboard(*this);
	removeSelection();
	auto pos = insertText(getCurrentLinePosition(), getCurrentCharPosition(),
			buf);
	setCurrentPosition(std::get<0>(pos), std::get<1>(pos));
}

void XEditorWindow::controlKey(wchar_t key) {

	switch(key) {
	case 'C':
		copySelection();
		break;
	case 'V':
		pasteAtCursor();
		break;
	case 'X':
		copySelection();
		removeSelection();
		break;
	}
}

std::vector<std::wstring>& XEditorWindow::getLines() {
	return lines_;
}

void XEditorWindow::setLine(int line, const std::wstring& str) {
	getLines()[line] = str;
	invalidateLine(line);
}

std::wstring StrTrimRight(const std::wstring& str) {
	int i;
	for(i = str.size() - 1; i >= 0 && isspace(str[i]); --i);
	return str.substr(0, i+1);
}

std::vector<std::wstring> split_lines(const std::wstring& text) {
	std::vector<std::wstring> lines;
	std::wstringstream ss {text};
	std::wstring tmp;

	while(!ss.eof()) {
		std::getline(ss, tmp);
		lines.push_back(StrTrimRight(tmp));
	}
	return lines;
}

void CopyToClipboard(Window &wnd, const std::wstring &text) {
    // Open and empty existing contents.
    if (OpenClipboard(wnd)) {
        if (EmptyClipboard()) {
            size_t sz = sizeof(wchar_t) * (text.size() + 1);
            HGLOBAL hClipboardData  = ::GlobalAlloc(GMEM_DDESHARE | GMEM_ZEROINIT, sz);

            if (hClipboardData != NULL) {
                void* memory = GlobalLock(hClipboardData);

                if (memory != nullptr) {
                    memcpy(memory, text.c_str(), sz);
                    GlobalUnlock(hClipboardData);

                    if (SetClipboardData(CF_UNICODETEXT, hClipboardData) != NULL)
                        hClipboardData = NULL; // system now owns the clipboard, so don't touch it.
                }
                GlobalFree(hClipboardData); // free if failed
            }
        }
        CloseClipboard();
    }
}

std::wstring PasteFromClipboard(Window &wnd) {
	std::wstring res;

    // Copy Unicode text from clipboard.
    if (OpenClipboard(wnd)) {
        HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);
        DWORD dwLE = GetLastError();
        if (hClipboardData != NULL) {
            // Get text and size of text.
            size_t sz = GlobalSize(hClipboardData);
            void* memory = GlobalLock(hClipboardData);
            const wchar_t* text = reinterpret_cast<const wchar_t*>(memory);

            if (memory != nullptr) {
            	res = text;
                GlobalUnlock(hClipboardData);
            }
        }
        CloseClipboard();
    }
    return res;
}

int XEditorWindow::getStdCharHeight() const {
	return stdCharHeight_;
}

void XEditorWindow::invalidateLine(int line) {
	invalidateLines(line, line+1);
}

void XEditorWindow::removeLine(int line) {
	getLines().erase(getLineIt(line));

	if(currentLine_ == line)
		moveToLineBegin();
	if(currentLine_ >= line)
		--currentLine_;

	repositionCaret();
	invalidateLines(line, getLines().size());
}

void XEditorWindow::setSelection(int startLine, int startPosition, int line,
		int position) {
	selecting_ = true;
	selStartLine_ = startLine;
	selStartChar_ = startPosition;
	currentLine_ = line;
	currentChar_ = position;
}

void XEditorWindow::startSelection() {
	selecting_ = true;
	selStartLine_ = getCurrentLinePosition();
	selStartChar_ = getCurrentCharPosition();
}

std::pair<int, int> XEditorWindow::getSelectionStart() const {
	if(selStartLine_ > currentLine_ ||
			(selStartLine_ == currentLine_ && selStartChar_ > currentChar_))
		return {currentLine_, currentChar_};
	else
		return {selStartLine_, selStartChar_};
}

std::pair<int, int> XEditorWindow::getSelectionEnd() const {
	if(selStartLine_ > currentLine_ ||
			(selStartLine_ == currentLine_ && selStartChar_ > currentChar_))
		return {selStartLine_, selStartChar_};
	else
		return {currentLine_, currentChar_};
}

bool XEditorWindow::hasSelection() const {
	return selStartLine_ != -1;
}

void XEditorWindow::selectDefFont() {
	LOGFONT font;
	memset(&font, 0, sizeof(font));

	wcscpy(font.lfFaceName, L"Consolas");
	font.lfHeight = -MulDiv(10, GetDeviceCaps(getDC(), LOGPIXELSY), 72);
	font.lfCharSet = DEFAULT_CHARSET;
	font.lfOutPrecision = OUT_OUTLINE_PRECIS;
	font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	font.lfQuality = CLEARTYPE_QUALITY;
	font.lfPitchAndFamily = VARIABLE_PITCH;
	selectFont(font);
}

void XEditorWindow::selectFont(LOGFONT font) {
	textFont_ = font;
	invalidateScreen();
}

void XEditorWindow::mouseMsg(UINT msg) {
	switch(msg) {
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
		break;
	}
}

void XEditorWindow::clearSelection() {
	invalidateLines(std::min(selStartLine_, currentLine_), std::max(selStartLine_, currentLine_) + 1);
	selecting_ = false;
	selStartLine_ = -1;
	selStartChar_ = -1;
}

inline bool Inbetween(int a, int b, int x) {
	return (a <= x && x < b) || (b <= x && x < a);
}

} /* namespace XEditor */

