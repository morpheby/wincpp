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

URLWnd::URLWnd() : visited_(false) {
}

URLWnd::URLWnd(const wstring& url, int x, int y, HWND parentWnd) :
		LabelWnd(url, x, y, parentWnd),
		visited_(false), urlFull_(url, 0, 0, *this) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

URLWnd::URLWnd(const wstring& url, const wstring& name, int x, int y,
		HWND parentWnd) :
		LabelWnd(name, x, y, parentWnd),
		visited_(false), url_(url), urlFull_(url, 0, 0, *this) {

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
	return visited_;
}

void URLWnd::PaintWindow(Drawing::Drawer &drawer) {
	COLORREF prevColor = visited_ ?
			drawer.setTextColor(RGB(0x58, 0x1C, 0x90)) :
			drawer.setTextColor(RGB(0, 0, 0xFF));

	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer)) {
		LOGFONT font;
		tDrawer->getThemeLogFont(TEXT_HYPERLINKTEXT, TS_HYPERLINK_NORMAL, font);
		font.lfUnderline = true;
		tDrawer->setFont(CreateFontIndirect(&font));
	}

	SetBkMode(drawer.getDC(), TRANSPARENT);
	drawer.drawText(txt, 0, txtRect);
	drawer.setFontDefault();
	drawer.setTextColor(prevColor);
}

LRESULT URLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	POINTS pt;
	RECT pos = {0,0,0,0};
	switch(msg) {
	case WM_LBUTTONUP:
		ShellExecuteW(*this, 0, getUrl().c_str(), 0, 0, SW_SHOWNORMAL);
		visited_ = true;
		UpdateWindow();
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
		break;
	case WM_MOUSEMOVE:
		TrackMouseEvent(TME_HOVER);
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
