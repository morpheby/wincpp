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

using namespace std;

URLWnd::URLWnd() : visited(false) {
}

URLWnd::URLWnd(const wstring& url, int x, int y, HWND parentWnd) :
		LabelWnd(url, x, y, parentWnd),
		visited(false), urlFull(url, 0, 0, *this) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

URLWnd::URLWnd(const wstring& url, const wstring& name, int x, int y,
		HWND parentWnd) :
		LabelWnd(name, x, y, parentWnd),
		visited(false), urlFull(url, 0, 0, *this) {

}

URLWnd::~URLWnd() {
}

const std::wstring& URLWnd::getUrl() const {
	if(url.empty())
		return getText(); // We have no full URL. Probably name itself is URL
	else
		return url;
}

void URLWnd::setUrl(const std::wstring& url) {
	this->url = url;
	if(url.empty())
		urlFull.setText(getText());
	else
		urlFull.setText(getUrl());
}

bool URLWnd::isVisited() const {
	return visited;
}

void URLWnd::PaintWindow(HDC hdc) {
	LOGFONT font;
	HFONT prevFont;
	COLORREF prevColor = visited ?
			SetTextColor(hdc, RGB(0x58, 0x1C, 0x90)) :
			SetTextColor(hdc, RGB(0, 0, 0xFF));
	if(GetThemeFontInt(TEXT_HYPERLINKTEXT, TS_HYPERLINK_NORMAL, font))
		// GetThemeFont failed. Use fall-back font
		prevFont = (HFONT) SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
	else {
		font.lfUnderline = true;
		prevFont = (HFONT) SelectObject(hdc, CreateFontIndirect(&font));
	}
	SetBkMode(hdc, TRANSPARENT);
	DrawTextW(hdc, txt.c_str(), -1, &txtRect, 0);
	DeleteObject(SelectObject(hdc, prevFont));
	SetTextColor(hdc, prevColor);
}

LRESULT URLWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	POINTS pt;
	RECT pos = {0,0,0,0};
	switch(msg) {
	case WM_LBUTTONUP:
		ShellExecuteW(*this, 0, getUrl().c_str(), 0, 0, SW_SHOWNORMAL);
		visited = true;
		UpdateWindow();
		break;
	case WM_SETCURSOR:
		SetCursor(LoadCursor(0, IDC_HAND));
		return 0; // Stop further processing
	case WM_MOUSEHOVER:
		pt = MAKEPOINTS(lParam);
		GetWindowRect(getWindowHandle(), &pos);
		urlFull.setPosition(pos.left + pt.x,
				pos.top + pt.y +
				CursorFactory::GetCursor().getBottom() + 2);
		urlFull.Show(SW_SHOWNA);
		TrackMouseEvent(TME_LEAVE);
		break;
	case WM_MOUSELEAVE:
		urlFull.Hide();
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
