
#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "ButtonWnd.h"

#if HAVE_WINDOWSX_H
#include <windowsx.h>
#endif

using namespace std;

ButtonWnd::ButtonWnd(void) :
		CommCtlWnd(WC_BUTTON) {
}

ButtonWnd::ButtonWnd(ButtonWnd::ButtonType btnType) :
		CommCtlWnd(L"", btnType, 0, 0, 0, 0, 0, WC_BUTTON) {
}

ButtonWnd::ButtonWnd(ButtonWnd::ButtonType btnType, wstring name, int x, int y,
		int width, int height, HWND parent) :
		CommCtlWnd(name, btnType, parent, x, y, width, height, WC_BUTTON) {
}

void ButtonWnd::SetButtonType(ButtonWnd::ButtonType btnType) {
	setStyle(btnType);
}

void ButtonWnd::setOnClick(WndEventBase* onClick) {
	this->onClick = onClick;
}

LRESULT ButtonWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT res = CommCtlWnd::WndProc(msg, wParam, lParam);
	if (msg == WM_LBUTTONUP && IsWindowEnabled(getWindowHandle())
			&& GET_X_LPARAM(lParam) <= getSize().cx
			&& GET_Y_LPARAM(lParam) <= getSize().cy)
		onClick(*this);
	return res;
}

ButtonWnd::~ButtonWnd(void) {
}
