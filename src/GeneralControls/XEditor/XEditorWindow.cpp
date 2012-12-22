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
#include <utf8.h>

#define NEWLINE L"\r\n"
#define TABSTR L"   "

namespace XEditor {

std::vector<std::wstring> split_lines(const std::wstring &text);
std::wstring PasteFromClipboard(Window &wnd);
void CopyToClipboard(Window &wnd, const std::wstring &text);
std::wstring StrTrimRight(const std::wstring& str); /// Removes any space characters from the right side of a string
std::wstring StrClean(const std::wstring& str); /// Cleans string from any control characters except \t
bool Inbetween(int a, int b, int x);

XEditorWindow::XEditorWindow() :
	Window(L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL/* | WS_HSCROLL*/, 0, 0, 0, 0, 0, 0, 0){
}

XEditorWindow::XEditorWindow(int x, int y, int width, int height,
		Window& parentWnd) :
	Window(L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL/* | WS_HSCROLL*/, x, y, width, height, parentWnd, 0, 0) {
	selectDefFont();
	lines_.push_back(L"");
	updateScrollInfo();
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
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetFocus(*this);
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		mouseMsg(msg, wParam);
		break;
	case WM_SETCURSOR:
		{
			POINT pt;
			GetCursorPos(&pt);
			MapWindowPoints(0, *this, &pt, 1);
			if(pt.x < getSizeX() && pt.y < getSizeY())
				textCursor_.setCurrent();
			else
				arrowCursor_.setCurrent();
			return 1;
		}
	case WM_VSCROLL:
		scrollMsg(msg, wParam);
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
		if(txtRect.bottom - txtRect.top < stdCharHeight_)
			txtRect.bottom = txtRect.top + stdCharHeight_;
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
	updateScrollInfo();
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

	return getText(getSelectionStart().first, getSelectionStart().second, getSelectionEnd().first, getSelectionEnd().second);
}

void XEditorWindow::removeSelection() {
	if(!hasSelection())
		return;

	int selectionStartLine = getSelectionStart().first,
		selectionStartChar = getSelectionStart().second,
		selectionEndLine = getSelectionEnd().first,
		selectionEndChar = getSelectionEnd().second;

	clearSelection();
	setCurrentPosition(selectionStartLine, selectionStartChar);

	removeText(selectionStartLine, selectionStartChar, selectionEndLine, selectionEndChar);
}

void XEditorWindow::invalidateScreen() {
	UpdateWindow();
}

void XEditorWindow::invalidateLines(int start, int end) {
	RECT updateRect {0, 0, getSizeX(), 0};

	if(start < 0)
		start = 0;
	if(end > getLines().size())
		end = getLines().size();

	updateRect.top = start * getStdCharHeight() - screenStartY_;
	if(end == getLines().size())
		updateRect.bottom = getSizeY();
	else
		updateRect.bottom = getStdCharHeight() * end - screenStartY_;

	UpdateWindow(&updateRect);
}

void XEditorWindow::PrePaintWindow(RECT& updateRect) {
	HideCaret(*this);
}

void XEditorWindow::PostPaintWindow(RECT& updateRect) {
	ShowCaret(*this);
}

void XEditorWindow::repositionCaret() {
    RECT txtRect {-screenStartX_, 0, 0, 0};
    calcLineRect(getCurrentLine().substr(0, currentChar_), txtRect);
    ::SetCaretPos(txtRect.right, getCurrentLinePosition() * stdCharHeight_ - screenStartY_);
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
		removeSelection();
		getLineIt(getCurrentLinePosition())->insert(getLineIt(getCurrentLinePosition())->begin() + getCurrentCharPosition(), ch);
		moveToNextChar();
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
	if(currentLine_ * stdCharHeight_ < screenStartY_) {
		screenStartY_ = currentLine_ * stdCharHeight_;
		scrollScreen();
	} else if ((currentLine_+1) * stdCharHeight_ > screenStartY_ + getSizeY()) {
		screenStartY_ = (currentLine_+1) * stdCharHeight_ - getSizeY();
		scrollScreen();
	}
	repositionCaret();
}

void XEditorWindow::setCurrentPosition(int line, int position) {
	setCurrentLinePosition(line);
	setCurrentCharPosition(position);
}

std::wstring& XEditorWindow::insertLine(int at, const std::wstring &line) {
	auto res = int_insertLine(at, line);

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

	for(int i = line+1, j = 1; j < lines.size(); ++i, ++j)
		int_insertLine(i, lines[j]);
//	getLines().insert(getLineIt(line+1), lines.begin() + 1, lines.end());
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
		if(shiftDown && !hasSelection())
			startSelection();
		else if(!shiftDown && hasSelection())
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
	case VK_TAB:
		getLineIt(getCurrentLinePosition())->insert(getCurrentCharPosition(), TABSTR);
		setCurrentCharPosition(getCurrentCharPosition() + 3);
		invalidateCurrentLine();
		break;
	default:
		break;
	}
}

void XEditorWindow::WMKeyUp(char keyCode) {
}

std::ostream& operator <<(std::ostream& stream,
		XEditor::XEditorWindow& xeditor) {
	for(auto line : xeditor.getLines())
		stream << utf8Str(StrTrimRight(line)) << std::endl;
	return stream;
}

std::istream& operator >>(std::istream& stream,
		XEditor::XEditorWindow& xeditor) {
	std::string line;
	while(!stream.eof()) {
		std::getline(stream, line);
		xeditor.insertLine(xeditor.getLines().size(), StrClean(wideStr(line)));
	}
	return stream;
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
	partDrawer.setTextColor(GetSysColor(COLOR_WINDOWTEXT));

	if(!bSimulate && selStart != -1) {
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

		HBRUSH br = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		partDrawer.fillRect(fillRect, br);
		DeleteObject(br);

		if(!bSimulate) {
			partDrawer.drawText(line.substr(0, selStart), 0, leftRect);
			partDrawer.setTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			if(selEnd != -1) {
				partDrawer.drawText(line.substr(selStart, selEnd - selStart), 0, fillRect);
				partDrawer.setTextColor(GetSysColor(COLOR_WINDOWTEXT));
				partDrawer.drawText(line.substr(selEnd), 0, rightRect);
			} else {
				partDrawer.drawText(line.substr(selStart), 0, fillRect);
				partDrawer.setTextColor(GetSysColor(COLOR_WINDOWTEXT));
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

std::wstring StrClean(const std::wstring& str) {
	std::wstring cleanStr;
	cleanStr.reserve(str.size());
	for(int i = 0; i < str.size(); ++i)
		if(isprint(str[i]))
			cleanStr += str[i];
		else if(str[i] == '\t')
			cleanStr += TABSTR;
	return cleanStr;
}

std::vector<std::wstring> split_lines(const std::wstring& text) {
	std::vector<std::wstring> lines;
	std::wstringstream ss {text};
	std::wstring tmp;

	while(!ss.eof()) {
		std::getline(ss, tmp);
		lines.push_back(StrClean(tmp));
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
	selStartLine_ = startLine;
	selStartChar_ = startPosition;
	currentLine_ = line;
	currentChar_ = position;
	invalidateLines(getSelectionStart().first, getSelectionEnd().first + 1);
}

void XEditorWindow::startSelection() {
	clearSelection();
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
	RECT t {0, 0, 0, 0};
	calcLineRect(L"Ay`,/|@#^_", t);
	stdCharHeight_ = t.bottom;
	invalidateScreen();
}

void XEditorWindow::mouseMsg(UINT msg, WPARAM wParam) {
	POINT pt;
	GetCursorPos(&pt);
	MapWindowPoints(NULL, *this, &pt, 1);

	switch(msg) {
	case WM_LBUTTONUP:
		ReleaseCapture();
		mouseHolding_ = false;
		if(hasSelection() && getSelectionStart() == getSelectionEnd())
			clearSelection();
		break;
	case WM_LBUTTONDOWN:
		setCurrentPosition(getPositionAt(pt));
		if(!(GetKeyState(VK_SHIFT) & 0x80))
			startSelection();
		mouseHolding_ = true;
		SetCapture(*this);
		break;
	case WM_MOUSEMOVE:
		if(mouseHolding_)
			setCurrentPosition(getPositionAt(pt));
		TrackMouseEvent(TME_LEAVE);
		break;
	case WM_MOUSELEAVE:
		arrowCursor_.setCurrent();
		break;
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		{
			// Retrieve the lines-to-scroll or characters-to-scroll user setting,
			// using a default value if the API failed.
			UINT userSetting;
			BOOL success = SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &userSetting, 0);
			if (success == FALSE)
				userSetting = 1;

			// Set x,y scroll difference,
			// depending on whether horizontal or vertical scroll.
			float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if(msg == WM_MOUSEWHEEL)
				mouseScroll(0, (zDelta / WHEEL_DELTA) * userSetting);
			else
				mouseScroll(- (zDelta / WHEEL_DELTA) * userSetting, 0);
		}
		break;
	}
}

void XEditorWindow::clearSelection() {
	invalidateLines(std::min(selStartLine_, currentLine_), std::max(selStartLine_, currentLine_) + 1);
	selStartLine_ = -1;
	selStartChar_ = -1;
}

inline bool Inbetween(int a, int b, int x) {
	return (a <= x && x < b) || (b <= x && x < a);
}

void XEditorWindow::mouseScroll(float scrollX, float scrollY) {
//	screenStartX_ += scrollX *;
	screenStartY_ -= scrollY * stdCharHeight_;
	scrollScreen();
}

std::pair<int, int> XEditorWindow::getPositionAt(POINT pt) {
	int line, position, diff = 0;

	line = std::max((long int)0, std::min((long int)getLines().size() - 1, (long int)(pt.y + screenStartY_) / getStdCharHeight()));

	int left = 0;
	diff = 0;
	for(position = 0; position < getLine(line).size() && left - screenStartX_ <= pt.x; ++position) {
		left += diff/2 + diff%2;
		RECT lineRect {0, 0, 0, 0};
		calcLineRect(getLine(line).substr(0, position + 1), lineRect);
		diff = lineRect.right - lineRect.left - left;
		left += diff/2;
	}
	if(left - screenStartX_ > pt.x)
		--position;

	return {line, position};
}

std::wstring XEditorWindow::processNewLine(int at, const std::wstring& line) {
	return line;
}

std::vector<std::wstring>::iterator XEditorWindow::int_insertLine(int at, const std::wstring& line) {
	auto res = getLines().insert(getLineIt(at), processNewLine(at, line));
	updateScrollInfo();
	return res;
}

void XEditorWindow::scrollMsg(UINT msg, WPARAM wParam) {
    SCROLLINFO scrollInfo = {sizeof(scrollInfo)};
    scrollInfo.fMask = SIF_ALL;

    int barOrientation = (msg == WM_VSCROLL) ? SB_VERT : SB_HORZ;

    if (!GetScrollInfo(*this, barOrientation, &scrollInfo) )
        return;

    switch (LOWORD(wParam)) {
    case SB_TOP:		screenStartY_  = scrollInfo.nMin;		break;
    case SB_BOTTOM:		screenStartY_  = scrollInfo.nMax;		break;
    case SB_LINEUP:		screenStartY_ -= stdCharHeight_;		break;
    case SB_LINEDOWN:	screenStartY_ += stdCharHeight_;		break;
    case SB_PAGEUP:		screenStartY_ -= scrollInfo.nPage;		break;
    case SB_PAGEDOWN:	screenStartY_ += scrollInfo.nPage;		break;
    case SB_THUMBTRACK:	screenStartY_  = scrollInfo.nTrackPos;	break;
    default:
         break;
    }

    scrollScreen();

}

void XEditorWindow::updateScrollInfo() {
    SCROLLINFO scrollInfo = {sizeof(scrollInfo)};
    scrollInfo.fMask    = SIF_PAGE|SIF_POS|SIF_RANGE;

    // Set vertical scroll bar.
    scrollInfo.nPage    = getSizeY();
    scrollInfo.nPos     = screenStartY_;
    scrollInfo.nMin     = 0;
    scrollInfo.nMax     = stdCharHeight_*getLines().size() + scrollInfo.nPage - stdCharHeight_;
    SetScrollInfo(*this, SB_VERT, &scrollInfo, TRUE);

    // XXX
//     Set horizontal scroll bar.
//    scrollInfo.nPage    = getSizeX();
//    scrollInfo.nPos     = screenStartX_;
//    scrollInfo.nMin     = 0;
//    scrollInfo.nMax     = stdCharHeight_*getLines().size();
//    SetScrollInfo(*this, SB_HORZ, &scrollInfo, TRUE);
}

void XEditorWindow::scrollScreen() {
	// XXX duplicate for X
    SCROLLINFO scrollInfo = {sizeof(scrollInfo)};
    scrollInfo.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;

    if (!GetScrollInfo(*this, SB_VERT, &scrollInfo) )
        return;


	if (screenStartY_ < 0)
		screenStartY_ = 0;
	if (screenStartY_ > scrollInfo.nMax - scrollInfo.nPage)
		screenStartY_ = scrollInfo.nMax - scrollInfo.nPage;
	scrollInfo.nPos = screenStartY_;
	scrollInfo.fMask = SIF_POS;
	SetScrollInfo(*this, SB_VERT, &scrollInfo, TRUE);

	repositionCaret();
	invalidateScreen();
}

void XEditorWindow::undo() {
}

void XEditorWindow::removeText(int startLine, int startPos, int endLine,
		int endPos) {
	setLine(startLine, getLine(startLine).substr(0, startPos) +
			getLine(endLine).substr(endPos));

	if(startLine != endLine) {
		getLines().erase(getLineIt(startLine + 1), getLineIt(endLine+1));
		invalidateLines(startLine, getLines().size());
	} else
		invalidateLine(startLine);

}

std::wstring XEditorWindow::getText(int startLine, int startCh, int endLine,
		int endCh) const {
	if(startLine == endLine)
		return getLine(startLine).substr(startCh, endCh - startCh);

	std::wstring selection;
	selection += getLine(startLine).substr(startCh) + NEWLINE;
	for(int i = startLine + 1; i < endLine; ++i)
		selection += getLine(i) + NEWLINE;
	selection += getLine(endLine).substr(0, endCh);

	return selection;
}

void XEditorWindow::redo() {
}

} /* namespace XEditor */
