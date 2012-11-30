/*
 * URLWnd.cpp
 *
 *  Created on: 31.05.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "URLWnd.h"
#include <Cursors.h>
#include <ThemedDrawer.h>

using namespace std;

URLWnd::URLWnd() : state_{0}, mBtnDown_{false} {
}

URLWnd::URLWnd(const wstring& url, int x, int y, HWND parentWnd) :
		LabelWnd(url, x, y, parentWnd),
		state_{0}, urlFull_(url, 0, 0, *this), mBtnDown_{false} {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

URLWnd::URLWnd(const wstring& url, const wstring& name, int x, int y,
		HWND parentWnd) :
		LabelWnd(name, x, y, parentWnd),
		state_{0}, url_{url}, urlFull_(url, 0, 0, *this), mBtnDown_{false} {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

URLWnd::~URLWnd() {
}

const std::wstring& URLWnd::getUrl() const {
	if(url_.empty())
		return getText(); // We have no full URL. Probably name itself is URL
	else
		return url_;
}

void URLWnd::setUrl(const std::wstring& url) {
	this->url_ = url;
	urlFull_.setText(getUrl());
}

bool URLWnd::isVisited() const {
	return state_ == 1;
}

void URLWnd::PaintWindow(Drawing::Drawer &drawer) {
	COLORREF color = state_ ?
			RGB(0x58, 0x1C, 0x90) :
			RGB(0, 0, 0xFF);

	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer)) {
		LOGFONT font = tDrawer->getThemeLogFont(TEXT_HYPERLINKTEXT, TS_HYPERLINK_NORMAL, TMT_FONT);
		font.lfUnderline = true;
		tDrawer->setFont(CreateFontIndirect(&font));
//		color = tDrawer->getThemeColor(TEXT_HYPERLINKTEXT, state_ == 1 ? TS_HYPERLINK_PRESSED :
//				TS_HYPERLINK_NORMAL, TMT_TEXTCOLOR); // those colors look ugly...
	}

	if(state_ < 0)
		color = ~color & 0x00FFFFFF;
	else if(state_ > 1)
		color -= 0x00000020;
	color = drawer.setTextColor(color);
	SetBkMode(drawer.getDC(), TRANSPARENT);
	drawer.drawText(txt, 0, txtRect);
	drawer.clearFont();
	drawer.setTextColor(color);
}

LRESULT URLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	POINTS pt;
	RECT pos = {0,0,0,0};
	switch(msg) {
	case WM_LBUTTONDOWN:
		state_ = -1;
		mBtnDown_ = true;
		TrackMouseEvent(TME_LEAVE);
		UpdateWindow();
		break;
	case WM_LBUTTONUP:
		if(mBtnDown_) {
			ShellExecuteW(*this, 0, getUrl().c_str(), 0, 0, SW_SHOWNORMAL);
			state_ = 1;
			UpdateWindow();
			mBtnDown_ = false;
		}
		break;
	case WM_SETCURSOR:
		SetCursor(LoadCursor(0, IDC_HAND));
		return 0; // Stop further processing
	case WM_MOUSEHOVER:
		pt = MAKEPOINTS(lParam);
		GetWindowRect(getWindowHandle(), &pos);
		urlFull_.setPosition(pos.left + pt.x,
				pos.top + pt.y +
				CursorFactory::GetCursor().getBottom() + 2);
		urlFull_.Show(SW_SHOWNA);
		TrackMouseEvent(TME_LEAVE);
		break;
	case WM_MOUSELEAVE:
		urlFull_.Hide();
		if(mBtnDown_) {
			state_ = 0;
			UpdateWindow();
		}
		break;
	case WM_MOUSEMOVE:
		if((mBtnDown_ && !(wParam & MK_LBUTTON)) || !mBtnDown_) {
			mBtnDown_ = false;
			TrackMouseEvent(TME_HOVER);
		} else if(mBtnDown_ && !state_) {
			state_ = -1;
			UpdateWindow();
			TrackMouseEvent(TME_LEAVE);
		}
		break;
	default:
		break;
	}

	return LabelWnd::WndProc(msg, wParam, lParam);
}

void URLWnd::TrackMouseEvent(DWORD event) {
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof tme;
	tme.hwndTrack = getWindowHandle();
	tme.dwHoverTime = HOVER_DEFAULT;
	tme.dwFlags = event;
	::TrackMouseEvent(&tme);
}

void URLWnd::CalcTxtRect() {
	if(isForcedWidth())
		GetClientRect(*this, &txtRect);
	else
		memset(&txtRect, 0, sizeof(txtRect));

	std::shared_ptr<Drawing::Drawer> drawerPtr = getDrawer();
	if(std::shared_ptr<Drawing::ThemedDrawer> tDrawer = std::dynamic_pointer_cast<Drawing::ThemedDrawer>(drawerPtr)) {
		LOGFONT font = tDrawer->getThemeLogFont(TEXT_HYPERLINKTEXT, TS_HYPERLINK_NORMAL, TMT_FONT);
		font.lfUnderline = true;
		tDrawer->setFont(CreateFontIndirect(&font));
	}

	drawerPtr->drawText(txt, ((isForcedWidth() ? DT_WORDBREAK : 0) | DT_CALCRECT), txtRect);
	drawerPtr->clearFont();
}
