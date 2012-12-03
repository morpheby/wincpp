/*
 * XEditorWindow.cpp
 *
 *  Created on: 02.12.2012
 *      Author: Morphe
 */

#include "XEditorWindow.h"

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
	currentLine_ = lines_.begin();
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
	drawer.setFont(CreateFontIndirect(&textFont_));
	for(auto line : lines_) {
		calcTxtRect(line, textFont_, txtRect);
		drawer.drawText(line, 0, txtRect);
		txtRect.top += maxCharHeight_;
	}
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

std::string XEditorWindow::getSelection() const {
}

void XEditorWindow::setSelection(std::wstring allocator) {
}

void XEditorWindow::invalidateScreen() {
	UpdateWindow();
}

void XEditorWindow::invalidateLine() {
	UpdateWindow();
}

void XEditorWindow::PrePaintWindow(RECT& updateRect) {
	HideCaret(*this);
}

void XEditorWindow::PostPaintWindow(RECT& updateRect) {
	ShowCaret(*this);
}

void XEditorWindow::repositionCaret() {
    RECT txtRect {0, 0, 0, 0};
    calcTxtRect(currentLine_->substr(0, currentChar_), textFont_, txtRect);
    ::SetCaretPos(txtRect.right, nCurrentLine_ * maxCharHeight_);
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
		setSelection(L"");
		currentLine_ = lines_.insert(++currentLine_, L"");
		++nCurrentLine_;
		currentChar_ = 0;
		repositionCaret();
		invalidateScreen();
		break;

	default:    // displayable character
		currentLine_->insert(currentLine_->begin()+currentChar_, ch);
		++currentChar_;
		repositionCaret();
		invalidateLine();
		break;
	}
}

void XEditorWindow::WMKeyDown(char keyCode) {
	switch(keyCode) {
	case VK_UP:
		if(currentLine_ != lines_.begin()) {
			--currentLine_;
			--nCurrentLine_;
		}
		if(currentChar_ > currentLine_->size())
			currentChar_ = currentLine_->size();
		break;
	case VK_DOWN:
		if(++currentLine_ == lines_.end())
			--currentLine_;
		else
			++nCurrentLine_;
		if(currentChar_ > currentLine_->size())
			currentChar_ = currentLine_->size();
		break;
	case VK_LEFT:
		if(currentChar_ > 0)
			--currentChar_;
		else if(currentLine_ != lines_.begin()) {
			--currentLine_;
			--nCurrentLine_;
			currentChar_ = currentLine_->size();
		}
		break;
	case VK_RIGHT:
		if(currentChar_ < currentLine_->size())
			++currentChar_;
		else if(++currentLine_ == lines_.end())
			--currentLine_;
		else {
			++nCurrentLine_;
			currentChar_ = 0;
		}
		break;
	case VK_HOME:
		currentChar_ = 0;
		break;
	case VK_END:
		currentChar_ = currentLine_->size();
		break;
	default:
		break;
	}
	repositionCaret();
}

} /* namespace XEditor */

std::ostream& operator <<(std::ostream& stream,
		XEditor::XEditorWindow& xeditor) {
}

std::istream& operator >>(std::istream& stream,
		XEditor::XEditorWindow& xeditor) {
}
