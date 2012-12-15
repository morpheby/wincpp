/*
 * XEditorWindow.cpp
 *
 *  Created on: 02.12.2012
 *      Author: Morphe
 */

#include "XEditorWindow.h"
#include <sstream>

namespace XEditor {

XEditorWindow::XEditorWindow() :
	Window(L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, 0, 0, 0){
}

XEditorWindow::XEditorWindow(int x, int y, int width, int height,
		Window& parentWnd) :
	Window(L"", WS_CHILD | WS_VISIBLE, x, y, width, height, parentWnd, 0, 0) {
	textFont_ = Drawing::Drawer::getLogFont((HFONT)GetStockObject(SYSTEM_FIXED_FONT));
	lines_.push_back(L"");
	RECT t {0, 0, 0, 0};
	calcTxtRect(L"A", textFont_, t); // initialize maxCharacterHeight_
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
		WMChar((wchar_t)wParam);
		break;
	case WM_KEYDOWN:
		WMKeyDown((char) wParam);
		break;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void XEditorWindow::PaintWindow(Drawing::Drawer& drawer) {
	RECT txtRect {0, 0, 0, 0};
	int top = 0;
	drawer.setFont(CreateFontIndirect(&textFont_));
	POINT org;
	GetWindowOrgEx(drawer.getDC(), &org);
//	HRGN oldClipRgn = CreateRectRgn(0, 0, 0, 0);
//	GetClipRgn(drawer.getDC(), oldClipRgn);
	for(auto line : getLines()) {
		calcTxtRect(line, textFont_, txtRect);
		SetWindowOrgEx(drawer.getDC(), org.x, org.y - top, 0);
//		HRGN newClipRgn = CreateRectRgnIndirect(&txtRect);
//		SelectClipRgn(drawer.getDC(), newClipRgn);
//		DeleteObject(newClipRgn);
		outputLine(drawer, line, txtRect);
		top += maxCharHeight_;
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

void XEditorWindow::calcTxtRect(const std::wstring &str, LOGFONT font, RECT &txtRect) {
	std::shared_ptr<Drawing::Drawer> drawerPtr = getDrawer();
	drawerPtr->setFont(CreateFontIndirect(&font));
	drawerPtr->drawText(str, DT_CALCRECT, txtRect);
	drawerPtr->clearFont();
	if(txtRect.bottom - txtRect.top > maxCharHeight_)
		maxCharHeight_ = txtRect.bottom - txtRect.top;
	else
		txtRect.bottom = txtRect.top + maxCharHeight_;
}

void XEditorWindow::WMSize(POINTS size) {
}

void XEditorWindow::WMKillFocus() {
    HideCaret(*this);
    DestroyCaret();
}

void XEditorWindow::WMSetFocus() {
    ::CreateCaret(*this, 0, 0, maxCharHeight_);
    repositionCaret();
    ::ShowCaret(*this);
}

std::wstring XEditorWindow::getSelection() const {
}

void XEditorWindow::replaceSelection(std::wstring selection) {
}

void XEditorWindow::invalidateScreen() {
	invalidatedLine_ = -1;
	UpdateWindow();
}

void XEditorWindow::invalidateLine() {
	RECT lineRect {0, maxCharHeight_ * (currentLine_ - screenStartLine_), 0, 0};
	if(lineRect.top < 0 || lineRect.top > getSizeY())
		return;
	invalidatedLine_ = currentLine_;
	calcTxtRect(getCurrentLine(), textFont_, lineRect);
	UpdateWindow(&lineRect);
}

void XEditorWindow::PrePaintWindow(RECT& updateRect) {
	HideCaret(*this);
}

void XEditorWindow::PostPaintWindow(RECT& updateRect) {
	ShowCaret(*this);
}

void XEditorWindow::repositionCaret() {
    RECT txtRect {0, 0, 0, 0};
    calcTxtRect(getCurrentLine().substr(0, currentChar_), textFont_, txtRect);
    ::SetCaretPos(txtRect.right, getCurrentLinePosition() * maxCharHeight_);

}

void XEditorWindow::WMChar(wchar_t ch) {
	switch (ch)
	{
	case 0x08:  // backspace
	case 0x0A:  // linefeed
	case 0x1B:  // escape
		MessageBeep((UINT) -1);
		break;

	case 0x0D:  // carriage return
		replaceSelection(L"");
//		insertLine(currentLine_ + 1, L"");
		insertText(getCurrentLinePosition(), getCurrentCharPosition(), L"\n");
		moveToNextLine();
		break;

	default:    // displayable character
		getLineIt(getCurrentLinePosition())->insert(getLineIt(getCurrentLinePosition())->begin() + getCurrentCharPosition(), ch);
		moveToNextChar();
		repositionCaret();
		invalidateLine();
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
	if(line < 0)
		currentLine_ = 0;
	else if(line >= getLines().size())
		currentLine_ = getLines().size() - 1;
	else
		currentLine_ = line;
	setCurrentCharPosition(0);
}

void XEditorWindow::setCurrentCharPosition(int position) {
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
	if(screenStartLine_ > at)
		++screenStartLine_;

	repositionCaret();
	invalidateScreen();
	return *res;
}

void XEditorWindow::insertText(int line, int position,
		const std::wstring& text) {
	std::vector<std::wstring> lines = split_lines(text);
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

	if(screenStartLine_ > line)
		screenStartLine_ += lines.size();

	repositionCaret();
	invalidateScreen();
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
	default:
		break;
	}
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
		const std::wstring& line, RECT txtRect) {
	partDrawer.drawText(line, 0, txtRect);
}

std::vector<std::wstring>& XEditorWindow::getLines() {
	return lines_;
}

void XEditorWindow::setLine(int line, const std::wstring& str) {
	getLines()[line] = str;
}

std::vector<std::wstring> split_lines(const std::wstring& text) {
	std::vector<std::wstring> lines;
	std::wstringstream ss {text};
	std::wstring tmp;
	while(!ss.eof()) {
		std::getline(ss, tmp);
		lines.push_back(tmp);
	}
	return lines;
}

} /* namespace XEditor */

