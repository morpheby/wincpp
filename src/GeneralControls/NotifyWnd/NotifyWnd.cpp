
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "NotifyWnd.h"

using namespace std;

NotifyWnd::NotifyWnd(void) :
		Window(L"", 0, 0, 0, 0, 0, 0, 0, 0), txt(L""), txtRect({5, 5, 0, 0}) {
}

NotifyWnd::NotifyWnd(const wstring &text, int x, int y, HWND parentWnd) :
		Window(L"", WS_POPUP, x, y, 0, 0, parentWnd, 0, 0), txt(text),
		txtRect({5, 5, 0, 0}) {
	setText(text);
}

void NotifyWnd::setText(const wstring &text) {
	txt = text;
	txtRect = {0, 0, 0, 0};
	HDC dc = GetDC(getWindowHandle());
	DrawTText(dc, TTP_BALLOON, TTBS_NORMAL, txt, DT_CALCRECT, txtRect);
	ReleaseDC(getWindowHandle(), dc);
	txtRect.left += 5; txtRect.right += 5;
	txtRect.top += 5; txtRect.bottom += 5;
	setSize(txtRect.right + 5, txtRect.bottom + 5);
	ImmediatelyUpdateWindow();
}

void NotifyWnd::PaintWindow(HDC hdc) {
	DrawTText(hdc, TTP_BALLOON, TTBS_NORMAL, txt, 0, txtRect);
}

NotifyWnd::~NotifyWnd(void) {
}

bool NotifyWnd::WMEraseBackground(HDC hdc) {
	RECT rect = {0, 0, getWidth(), getHeight()};
	DrawTBackground(hdc, TTP_BALLOON, TTBS_NORMAL, rect);
	return true;
}

wstring NotifyWnd::GetThemeApplicableClassList() {
	return wstring(TOOLTIPS_CLASSW) + L";TOOLTIP;" + Window::GetThemeApplicableClassList();
	// TOOLTIPS_CLASS does not always work
}

