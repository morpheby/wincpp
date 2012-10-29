
#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "Window.h"
#include "DCFactory.h"
#include "ThemedDrawer.h"

using namespace std;

#define MYWNDCLASS_NAME TEXT("MyWndClass")

bool Window::wndClassRegistered = false;
unsigned int Window::wndCreating = 0;

Window::Window(void) {
	PostWindowCreate(L"", 0, 0, 0, 0, 0, 0, 0, 0);
}

Window::Window(bool tagDelayCreate) {
}

Window::Window(const wstring& wndName, DWORD style, int x, int y, int width,
		int height, HWND parentWindow, HMENU menu, LPVOID lpParam) {
	PostWindowCreate(wndName, style, x, y, width, height, parentWindow, menu,
			lpParam);
}

void Window::InitVars() {
	cachedBmp = 0;
	::SetLastError(0);
	defMsgProc = false;
	cacheOn_ = true;
	hTheme_ = 0;
}

//void Window::ReloadSize() {
//	// Reload width and height
//	RECT clRect;
//	GetClientRect(*this, &clRect);
//	width_ = clRect.right;
//	height_ = clRect.bottom;
//}

void Window::AssignClass() {
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) this);
}

//void Window::ReloadPos() {
//	RECT rc = { 0, 0, 0, 0 };
//	GetWindowRect(*this, &rc);
//	MapWindowPoints(0, getParent(), (POINT *) &rc, 2);
//	x_ = rc.left;
//	y_ = rc.top;
//}

void Window::PostWindowCreate(const wstring& wndName, DWORD style, int x, int y,
		int width, int height, HWND parentWindow, HMENU menu, LPVOID lpParam) {
	if (!wndClassRegistered)
		RegisterWndClass();

	InitVars();

	++wndCreating;
	hWnd = CreateWnd(wndName, style | WS_CLIPSIBLINGS,
			x, y, width, height, parentWindow, menu,
			::GetModuleHandle(NULL), lpParam);
	if (!hWnd) {
		--wndCreating;
		return;
	}

	AssignClass();

	--wndCreating;
	UpdateWindow();
}

HWND Window::CreateWnd(const wstring& wndName, UINT style,
		int x, int y, int width, int height, HWND parentWnd, HMENU menu,
		HINSTANCE instance, LPVOID lpParam) {
	if( ! CheckScreenResolution(width, height) )
		return 0;
	else
		return ::CreateWindow(MYWNDCLASS_NAME, wndName.c_str(), style, x, y,
				width, height, parentWnd, menu, instance, lpParam);
}

Window::~Window(void) {
	if (hWnd)
		DestroyWindow(hWnd);
}
void Window::setDefMsgProcessing() {
	defMsgProc = true;
}

void Window::resetDefMsgProcessing() {
	defMsgProc = false;
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	int processed = false;
	LRESULT retVal = 0;
	HRGN updRgn = 0;
	bool wndDestroy = false;

	if(defMsgProc && (msg == WM_PAINT || msg == WM_ERASEBKGND)) {
		if(msg == WM_PAINT) {
			// preserve update region
			updRgn = CreateRectRgn(0, 0, 0, 0);
			GetUpdateRgn(*this, updRgn, 0);
		}
		retVal = DefWndProc(msg, wParam, lParam);
	}

	switch (msg) {
	case WM_COMMAND:
		if(lParam && GetParent((HWND) lParam) != *this) {
			// Redirect message to the new parent
			retVal = SendMessage(GetParent((HWND)lParam), WM_COMMAND, wParam, lParam);
			processed = retVal;
		}
		break;
	case WM_PAINT:
		if(defMsgProc) {
			// restore preserved region
			InvalidateRgn(*this, updRgn, 0);
			DeleteObject(updRgn);
		}
		IntPaintWindow();
		processed = true;
		break;
	case WM_PRINTCLIENT:
		BitBlt((HDC)wParam, 0, 0, getSize().cx, getSize().cy,
				DC::GetDC(*this), 0, 0, SRCCOPY);
		processed = true;
		break;
	case WM_CTLCOLORBTN:
		retVal = (int) COLOR_WINDOW;
		processed = true;
		break;
	case WM_ERASEBKGND:
		processed = true;
		if(!cacheOn_ && !defMsgProc) {
			if(hTheme_) {
				Drawing::ThemedDrawer drawer ((HDC)wParam, hTheme_);
				WMEraseBackground(drawer);
			} else {
				Drawing::Drawer drawer ((HDC)wParam);
				WMEraseBackground(drawer);
			}
		}
		retVal = 1;
		break;
	case WM_WINDOWPOSCHANGED:
		if(! (((WINDOWPOS*)lParam)->flags & SWP_NOSIZE) )
			update_ = true;
		break;
	case WM_DESTROY:
		hWnd = 0;
		wndDestroy = true; // mark as possibly no more existent
		// pass it to defproc
		break;
	case WM_THEMECHANGED:
		CloseThemeData(hTheme_);
		OpenTheme();
		break;
	default:
		break;
	}

	WinMessage_t winMsg = {msg, wParam, lParam, 0};
	processed = CallMsgProc(winMsg);
	retVal = winMsg.retVal;

	if(wndDestroy || !IsWindow(hWnd))
		return 0;
		// We may be no longer able to use virtual functions
//		return DefWindowProc(getWindowHandle(), msg, wParam, lParam);

	return defMsgProc && (msg == WM_PAINT || msg == WM_ERASEBKGND) ? retVal :
			processed ? retVal : DefWndProc(msg, wParam, lParam);
}

LRESULT Window::DefWndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(getWindowHandle(), msg, wParam, lParam);
}

bool Window::CheckScreenResolution(int width, int height) {
	if(width > GetSystemMetrics(SM_CXSCREEN) || height > GetSystemMetrics(SM_CYSCREEN)) {
		MessageBeep(MB_ICONWARNING);
		return MessageBoxW(GetActiveWindow(), L"Warning! Current screen resolution is not recommended for this application.\n"
				"Do you want to continue?", L"Warning", MB_ICONWARNING | MB_YESNO) == IDYES;
	} else
		return true;
}

void Window::setPainter(WndEventExtBase<Drawing::Drawer&>* painter) {
	painter_ = painter;
}

void Window::setProcessMessage(UINT msg,
		WndEventExtBase<WinMessage_t&>* msgProc) {
	msgMap_[msg] = msgProc;
}

DC::DeviceContext Window::getDC() {
	return DC::GetDC(*this);
}

std::shared_ptr<Drawing::Drawer> Window::getDrawer() {
	if(hTheme_)
		return SharePtr(new Drawing::ThemedDrawer(getDC(), hTheme_));
	else
		return SharePtr(new Drawing::Drawer(getDC()));
}

BOOL Window::setSize(SIZE sz) {
	return setSize(sz.cx, sz.cy);
}

BOOL Window::setPosition(POINT pos) {
	return setPosition(pos.x, pos.y);
}

int Window::CallMsgProc(WinMessage_t& msg) {
	return msgMap_[msg.msg] (*this, msg);
}

void Window::clearMessageMap() {
	msgMap_.clear();
}

void Window::IntCachedPaint(DC::DeviceContext dc, RECT updateRect) {
	if (NeedsUpdate()) {
		DC::DeviceContext cacheDC = DC::CreateCompatibleDC(dc);
		HBITMAP bmp = CreateCompatibleBitmap(dc, getSize().cx, getSize().cy);
		bmp = (HBITMAP) cacheDC.selectObject(bmp);
		// First perform internal painting, then external
		if(hTheme_) {
			Drawing::ThemedDrawer drawer (cacheDC, hTheme_);
			WMEraseBackground(drawer);
			PaintWindow(drawer);
			painter_(*this, drawer);
		} else {
			Drawing::Drawer drawer (cacheDC);
			WMEraseBackground(drawer);
			PaintWindow(drawer);
			painter_(*this, drawer);
		}
		delete cachedBmp;
		cachedBmp = new Bitmap(cacheDC,
				bmp = (HBITMAP) cacheDC.selectObject(bmp));
		DeleteObject(bmp);
	}
	DC::DeviceContext cacheDC = DC::CreateCompatibleDC(dc);
	HBITMAP bmp = (HBITMAP) cacheDC.selectObject(cachedBmp->CreateDDB(dc));
	BitBlt(dc, updateRect.left, updateRect.top, updateRect.right,
			updateRect.bottom, cacheDC, updateRect.left, updateRect.top,
			SRCCOPY);
	DeleteObject(cacheDC.selectObject(bmp));
}

void Window::IntPaintWindow() {
	RECT updateRect;
	if (GetUpdateRect(getWindowHandle(), &updateRect, 0) != 0) {
		PrePaintWindow(&updateRect);
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(getWindowHandle(), &ps);

		if(cacheOn_)
			IntCachedPaint(dc, updateRect);
		else {
			// First perform internal painting, then external
			if(hTheme_) {
				Drawing::ThemedDrawer drawer (dc, hTheme_);
				PaintWindow(drawer);
				painter_(*this, drawer);
			} else {
				Drawing::Drawer drawer (dc);
				PaintWindow(drawer);
				painter_(*this, drawer);
			}
		}

		EndPaint(getWindowHandle(), &ps);
	}
}

wstring Window::GetThemeApplicableClassList() {
	return L"DIALOG;WINDOW"; // Dialog themes are usually cooler than Window ones ;)
}

//HPEN Window::CreateCosmeticPen(COLORREF color) {
//	LOGBRUSH lbr;
//	lbr.lbHatch = 0;
//	lbr.lbColor = color;
//	lbr.lbStyle = BS_SOLID;
//	return ExtCreatePen(PS_COSMETIC | PS_SOLID, 1, &lbr, 0, 0);
//}
//
//HRESULT Window::DrawTText(HDC hdc, int iPartId, int iStateId,
//		const wstring& text, DWORD dwFlags, RECT& rect) {
//	OpenTheme();
//#if WINVER>=0x600
//	if(hTheme)
//		return DrawThemeTextEx(hTheme, hdc, iPartId, iStateId, text.c_str(),
//				text.size(), dwFlags, &rect, 0);
//#else
//	if(dwFlags & DT_CALCRECT)
//		return GetThemeTextExtent(hTheme, hdc, iPartId, iStateId,
//				text.c_str(), text.size(), dwFlags, rect.left || rect.right ? &rect : 0, &rect);
//	else if(hTheme)
//		return DrawThemeText(hTheme, hdc, iPartId, iStateId, text.c_str(),
//				text.size(), dwFlags, 0, &rect);
//	else
//		// fall-back mode
//		return DrawTextW(hdc, text.c_str(), -1, &rect, dwFlags);
//#endif
//}
//
//HRESULT Window::DrawTText(HDC hdc, const wstring& text, DWORD dwFlags, RECT& rect) {
//	return DrawTText(hdc, 0, 0, text, dwFlags, rect);
//}
//
//HRESULT Window::DrawTBackground(HDC hdc, int iPartId, int iStateId,
//		RECT& rect) {
//	return DrawTBackground(hdc, iPartId, iStateId, rect, 0);
//}
//
//HRESULT Window::DrawTBackground(HDC hdc, int iPartId, int iStateId,
//		RECT& rect, RECT *clipRect) {
//	OpenTheme();
//
//	if(!hTheme)
//		// fall-back mode
//		return FillRect(hdc, &rect, (HBRUSH) GetStockObject(WHITE_BRUSH));
//
//	if (IsThemeBackgroundPartiallyTransparent(hTheme, iPartId, iStateId))
//		DrawThemeParentBackground(getWindowHandle(), hdc, 0);
//
//	return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, &rect, clipRect);
//}

void Window::OpenTheme() {
	if(!hTheme_)
		hTheme_ = OpenThemeData(getWindowHandle(), GetThemeApplicableClassList().c_str());
}

Window* Window::SafeWindowFromHandle(const HWND wnd) {
	Window *pwnd;
	try {
		pwnd = static_cast<Window*>((void*) GetWindowLongPtr(wnd,
						GWLP_USERDATA));
		if (!pwnd || wnd != pwnd->getWindowHandle())
			return 0;
	} catch (...) {
		return 0;
	}
	return pwnd;
}

bool Window::WMEraseBackground(Drawing::Drawer &drawer) {
	RECT rc = getClientRect();
	drawer.drawBackground(rc);
	return false;
}

void Window::setFont(HFONT font) {
	::SendMessage(*this, WM_SETFONT, (WPARAM) font, 1);
}

void Window::RegisterWndClass() {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = IntWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = MYWNDCLASS_NAME;
	wcex.hIconSm = NULL;

	wndClassRegistered = (::RegisterClassEx(&wcex) != 0);
}

LRESULT __stdcall Window::IntWndProc(HWND wnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {
	Window *pwnd = static_cast<Window*>((void*) ::GetWindowLongPtr(wnd,
			GWLP_USERDATA));
	try {
		if (pwnd && wnd == pwnd->getWindowHandle() && !wndCreating)
			return pwnd->WndProc(msg, wParam, lParam);
	} catch (...) {
		// Possible bug: if we got there, than it is possible that
		// window is invalid. However it is quite normal in process
		// of window destruction - so we shall just fall back to
		// the default API. Destroying that window in any way may
		// result in deep recursion and/or segfaults
		return ::DefWindowProc(wnd, msg, wParam, lParam);
	}
	return ::DefWindowProc(wnd, msg, wParam, lParam);
}

BOOL Window::Show() {
	return Show(SW_SHOW);
}

BOOL Window::Hide() {
	return Show(SW_HIDE);
}

BOOL Window::Show(int type) {
	BOOL res = ::ShowWindow(getWindowHandle(), type);
	return res;
}

BOOL Window::SetWindowPos(HWND insertAfter, int x, int y, int width, int height,
		UINT flags) {
	return ::SetWindowPos(getWindowHandle(), insertAfter, x, y,
			width, height, flags);
}

BOOL Window::setName(const wstring& name) {
	return ::SetWindowText(getWindowHandle(), name.c_str());
}

wstring Window::getName() const {
	size_t txtLength = ::SendMessageW(getWindowHandle(), WM_GETTEXTLENGTH, 0, 0) + 1;
	wchar_t *buff = new wchar_t[txtLength];
	::GetWindowTextW(getWindowHandle(), buff, txtLength);
	wstring text(buff);
	delete[] buff;
	return text;
}

BOOL Window::setPosition(int x, int y) {
	return SetWindowPos(0, x, y, 0 ,0, SWP_NOACTIVATE | SWP_NOSIZE);
}

BOOL Window::setSize(int width, int height) {
	return SetWindowPos(0, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOMOVE);
}

RECT Window::getWindowRect() const {
	RECT r;
	::GetWindowRect(getWindowHandle(), &r);
	return r;
}

RECT Window::getClientRect() const {
	RECT r;
	::GetClientRect(getWindowHandle(), &r);
	return r;
}

SIZE Window::getSize() const {
	return SIZE{getClientRect().right, getClientRect().bottom};
}

POINT Window::getPosition() const {
	POINT pos = {getWindowRect().left, getWindowRect().top};
	MapWindowPoints(0, getParent(), (POINT *) &pos, 1);
	return pos;
}

BOOL Window::setMenu(HMENU hMenu) {
	return ::SetMenu(getWindowHandle(), hMenu);
}

HMENU Window::getMenu() const {
	return ::GetMenu(getWindowHandle());
}

LONG_PTR Window::setStyle(LONG_PTR style) {
	LONG_PTR res = ::SetWindowLongPtr(getWindowHandle(), GWL_STYLE,
			getStyle() | style);
	//Setting style does not change anything before we actually move window
	SetWindowPos(0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return res;
}

LONG_PTR Window::getStyle() const {
	return ::GetWindowLongPtr(getWindowHandle(), GWL_STYLE);
}

LONG_PTR Window::clearStyle(LONG_PTR clearBits) {
	LONG_PTR res = ::SetWindowLongPtr(getWindowHandle(), GWL_STYLE,
			getStyle() & ~clearBits);
	// Setting style does not change anything before we actually move window
	SetWindowPos(0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return res;
}

LONG_PTR Window::clearStyleEx(LONG_PTR clearBits) {
	LONG_PTR res = ::SetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE,
			getStyleEx() & ~clearBits);
	// Setting style does not change anything before we actually move window
	SetWindowPos(0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return res;
}

LONG_PTR Window::setStyleEx(LONG_PTR styleEx) {
	LONG_PTR res = ::SetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE,
			getStyleEx() | styleEx);
	// Setting style does not change anything before we actually move window
	SetWindowPos(0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	return res;
}

LONG_PTR Window::getStyleEx() const {
	return ::GetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE);
}

HWND Window::setParent(HWND parentWindow) {
	HWND res = ::SetParent(getWindowHandle(), parentWindow);
	// XXX MSDN recommends synchronization of UISTATE through WM_CHANGEUISTATE
	return res;
}

HWND Window::getParent() const {
	return ::GetParent(getWindowHandle());
}

void Window::UpdateWindow() {
	UpdateWindow(0);
}

void Window::UpdateWindow(RECT *updateRect) {
	update_ = true;
	::InvalidateRect(getWindowHandle(), updateRect, 1);
}

void Window::ImmediatelyUpdateWindow() {
	UpdateWindow();
	::UpdateWindow(getWindowHandle());
}

Window *Window::SafeGetWindow(UINT wCmd) {
	HWND wnd = ::GetWindow(getWindowHandle(), wCmd);
	if (!wnd)
		return 0;
	return SafeWindowFromHandle(wnd);
}

void Window::setPaintCachingMode(bool cacheOn) {
	cacheOn_ = cacheOn;
}

void Window::setRedraw(bool redraw) {
	::SendMessage(getWindowHandle(), WM_SETREDRAW, redraw, 0);
}
