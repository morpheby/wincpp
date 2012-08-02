
#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "CommCtlWnd.h"

using namespace std;

CommCtlWnd::CommCtlWnd(wstring wndClass) :
		Window(true), commCtlClass(wndClass), converted(0) {
	PostWindowCreate(L"", WS_VISIBLE, 0, 0, 0, 0, 0, 0, 0);
	setDefMsgProcessing();
	setPaintCachingMode(false);
	setDefFont();
	SendMessage(*this, CCM_DPISCALE, 1, 0);
}

CommCtlWnd::CommCtlWnd(wstring name, UINT style, HWND wndParent, int x, int y,
		int width, int height, wstring wndClass) :
		Window(true), commCtlClass(wndClass), converted(0) {
	PostWindowCreate(name, style | WS_VISIBLE | WS_CHILD, x, y,
			width, height, wndParent, 0, 0);
	setDefMsgProcessing();
	setPaintCachingMode(false);
	setDefFont();
	SendMessage(*this, CCM_DPISCALE, 1, 0);
}

CommCtlWnd::CommCtlWnd(HWND convertFrom) : Window(true),
		converted(convertFrom) {
	// Max. className length for Win32 is 256
	wchar_t className[257] = L"";
	GetClassNameW(convertFrom, className, 257);
	commCtlClass = className;

	// Perform typical initialization procedures
	PostWindowCreate(L"", 0, 0, 0, 0, 0, 0, 0, 0);

	// Init CommCtlWnd class
	setDefMsgProcessing();
	setPaintCachingMode(false);
	setDefFont();
	SendMessage(*this, CCM_DPISCALE, 1, 0);
}

LRESULT CommCtlWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if(inhibitedMsg.count(msg))
		return 0;
	return Window::WndProc(msg, wParam, lParam);
}

LRESULT CommCtlWnd::DefWndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	if (commCtlProc)
		return CallWindowProc(commCtlProc, getWindowHandle(), msg, wParam,
				lParam);
	else
		return Window::DefWndProc(msg, wParam, lParam);
}

wstring CommCtlWnd::GetThemeApplicableClassList() {
	return L"TEXTSTYLE;" + commCtlClass + L";" +
			Window::GetThemeApplicableClassList();
}

HWND CommCtlWnd::CreateWnd(const wstring& wndName, UINT style, HWND parentWnd, HMENU menu, HINSTANCE instance,
		LPVOID lpParam) {
	HWND res = 0;
	if(converted)
		res = converted;
	else
		res = CreateWindow(commCtlClass.c_str(), wndName.c_str(), style,
				getX(), getY(), getWidth(), getHeight(), parentWnd,
				menu, instance, lpParam);
	commCtlProc = (WNDPROC)
			SetWindowLongPtr(res, GWLP_WNDPROC,	(LONG_PTR) IntWndProc);
	if(commCtlProc == IntWndProc) {
		commCtlProc = 0;
		if(ShowAppError(L"Attempt to subclass the window of the same class.\n"
				"Continue?", L"Critical error",
				MB_ICONERROR | MB_YESNO) == IDYES)
			return 0;
		else
			exit(ERROR_ALREADY_INITIALIZED);
	}
	return res;
}

CommCtlWnd::~CommCtlWnd(void) {
}

void CommCtlWnd::WMKeyDown(int vk) {
	switch (vk)
	{
	case VK_TAB:
		SendMessage(getParent(), WM_TAB, 0, 0);
		break;
	case VK_ESCAPE:
		SendMessage(getParent(), WM_ESC, 0, 0);
		break;
	case VK_RETURN:
		SendMessage(getParent(), WM_ENTER, 0, 0);
		break;
	}
}

void CommCtlWnd::setDefFont() {
	setFont(GetThemeFont(TEXT_BODYTEXT, 0));
}

void CommCtlWnd::InhibitMessage(UINT msg) {
	inhibitedMsg.insert(msg);
}

void CommCtlWnd::RestoreMessage(UINT msg) {
	inhibitedMsg.erase(msg);
}
