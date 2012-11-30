
#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "Window.h"

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

void Window::InitVars(int x, int y, int width, int height) {
	hTheme = 0;
	cachedBmp = 0;
	SetLastError(0);
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;
	defMsgProc = false;
	cacheOn_ = true;
}

void Window::ReloadSize() {
	// Reload width and height
	RECT clRect;
	GetClientRect(*this, &clRect);
	width_ = clRect.right;
	height_ = clRect.bottom;
}

void Window::AssignClass() {
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) this);
}

void Window::ReloadPos() {
	RECT rc = { 0, 0, 0, 0 };
	GetWindowRect(*this, &rc);
	MapWindowPoints(0, getParent(), (POINT *) &rc, 2);
	x_ = rc.left;
	y_ = rc.top;
}

void Window::PostWindowCreate(const wstring& wndName, DWORD style, int x, int y,
		int width, int height, HWND parentWindow, HMENU menu, LPVOID lpParam) {
	if (!wndClassRegistered)
		RegisterWndClass();

	InitVars(x, y, width, height);

	++wndCreating;
	hWnd = CreateWnd(wndName, style | WS_CLIPSIBLINGS, parentWindow, menu, GetModuleHandle(NULL),
			lpParam);
	if (!hWnd) {
		--wndCreating;
		return;
	}

	// Reload width and height
	ReloadSize();
	ReloadPos();

	AssignClass();

	--wndCreating;
	UpdateWindow();
}

HWND Window::CreateWnd(const wstring& wndName, UINT style, HWND parentWnd, HMENU menu, HINSTANCE instance,
		LPVOID lpParam) {
	if( ! CheckScreenResolution(getWidth(), getHeight()) )
		return 0;
	else
		return CreateWindow(MYWNDCLASS_NAME, wndName.c_str(), style, getX(), getY(),
				getWidth(), getHeight(), parentWnd, menu, instance, lpParam);
}

Window::~Window(void) {
	CloseThemeData(hTheme);
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
	int retVal = 0;
	HDC dc;
	HRGN updRgn = 0;

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
		// if event doen't do anything good, use internal processing
		if(!onPaintWindow(*this))
			IntPaintWindow();
		processed = true;
		break;
	case WM_PRINTCLIENT:
		BitBlt((HDC)wParam, 0, 0, getWidth(), getHeight(),
				dc = GetDC(getWindowHandle()), 0, 0, SRCCOPY);
		processed = true;
		ReleaseDC(getWindowHandle(), dc);
		break;
	case WM_CTLCOLORBTN:
		retVal = (int) COLOR_WINDOW;
		processed = true;
		break;
	case WM_ERASEBKGND:
		processed = true;
		if(!cacheOn_ && !defMsgProc)
			WMEraseBackground((HDC)wParam);
		retVal = 1;
		break;
	case WM_CLOSE:
		// We call both internal class function and
		// event handler
		processed = WMClose() | onClose(*this) ;
		break;
	case WM_MOVE:
		ReloadPos();
		ReloadSize(); // If movement occured together with sizing,
					  // than our size data may become outdated
		WMMove();
		break;
	case WM_SIZE:
		ReloadPos();
		ReloadSize();
		update_ = true;
		WMSize();
		break;
	case WM_SHOWWINDOW:
		onShowWindow(*this);
		break;
	case WM_THEMECHANGED:
		CloseThemeData(hTheme);
		OpenTheme();
		// pass it to defproc
		break;
	case WM_DESTROY:
		onDestroy_(*this);
		WMDestroy();
		hWnd = 0;
		// pass it to defproc
		break;
	case WM_SETFOCUS:
		if((processed = WMSetFocus()) || defMsgProc)
			onSetFocus(*this);
		break;
	case WM_KILLFOCUS:
		if((processed = WMKillFocus()) || defMsgProc)
			onKillFocus(*this);
		break;
	case WM_KEYDOWN:
		WMKeyDown(wParam);
		break;
	case WM_CHAR:
		WMChar((wchar_t)wParam);
		break;
	default:
		break;
	}

	if(hWnd == 0)
		// We may be no longer able to use virtual functions
		return DefWindowProc(getWindowHandle(), msg, wParam, lParam);

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

void Window::IntCachedPaint(HDC dc, RECT updateRect) {
	if (NeedsUpdate()) {
		HDC cacheDC = CreateCompatibleDC(dc);
		HBITMAP bmp = CreateCompatibleBitmap(dc, getWidth(), getHeight());
		bmp = (HBITMAP)(SelectObject(cacheDC, bmp));
		WMEraseBackground(cacheDC);
		PaintWindow(cacheDC);
		delete cachedBmp;
		cachedBmp = new Bitmap(cacheDC,
				bmp = (HBITMAP)(SelectObject(cacheDC, bmp)));
		DeleteObject(bmp);
		DeleteDC(cacheDC);
	}
	HDC cacheDC = CreateCompatibleDC(dc);
	HBITMAP bmp = (HBITMAP)(SelectObject(cacheDC, cachedBmp->CreateDDB(dc)));
	BitBlt(dc, updateRect.left, updateRect.top, updateRect.right,
			updateRect.bottom, cacheDC, updateRect.left, updateRect.top,
			SRCCOPY);
	DeleteObject(SelectObject(cacheDC, bmp));
	DeleteDC(cacheDC);
}

void Window::IntPaintWindow() {
	RECT updateRect;
	if (GetUpdateRect(getWindowHandle(), &updateRect, 0) != 0) {
		PrePaintWindow(&updateRect);
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(getWindowHandle(), &ps);

		if(cacheOn_)
			IntCachedPaint(dc, updateRect);
		else
			PaintWindow(dc);

		EndPaint(getWindowHandle(), &ps);
	}
}

wstring Window::GetThemeApplicableClassList() {
	return L"DIALOG;WINDOW"; // Dialog themes are usually cooler than Window ones ;)
}

HPEN Window::CreateCosmeticPen(COLORREF color) {
	LOGBRUSH lbr;
	lbr.lbHatch = 0;
	lbr.lbColor = color;
	lbr.lbStyle = BS_SOLID;
	return ExtCreatePen(PS_COSMETIC | PS_SOLID, 1, &lbr, 0, 0);
}

HRESULT Window::DrawTText(HDC hdc, int iPartId, int iStateId,
		const wstring& text, DWORD dwFlags, RECT& rect) {
	OpenTheme();
#if WINVER>=0x600
	if(hTheme)
		return DrawThemeTextEx(hTheme, hdc, iPartId, iStateId, text.c_str(),
				text.size(), dwFlags, &rect, 0);
#else
	if(dwFlags & DT_CALCRECT)
		return GetThemeTextExtent(hTheme, hdc, iPartId, iStateId,
				text.c_str(), text.size(), dwFlags, rect.left || rect.right ? &rect : 0, &rect);
	else if(hTheme)
		return DrawThemeText(hTheme, hdc, iPartId, iStateId, text.c_str(),
				text.size(), dwFlags, 0, &rect);
	else
		// fall-back mode
		return DrawTextW(hdc, text.c_str(), -1, &rect, dwFlags);
#endif
}

HRESULT Window::DrawTText(HDC hdc, const wstring& text, DWORD dwFlags, RECT& rect) {
	return DrawTText(hdc, 0, 0, text, dwFlags, rect);
}

HRESULT Window::DrawTBackground(HDC hdc, int iPartId, int iStateId,
		RECT& rect) {
	return DrawTBackground(hdc, iPartId, iStateId, rect, 0);
}

HRESULT Window::DrawTBackground(HDC hdc, int iPartId, int iStateId,
		RECT& rect, RECT *clipRect) {
	OpenTheme();

	if(!hTheme)
		// fall-back mode
		return FillRect(hdc, &rect, (HBRUSH) GetStockObject(WHITE_BRUSH));

	if (IsThemeBackgroundPartiallyTransparent(hTheme, iPartId, iStateId))
		DrawThemeParentBackground(getWindowHandle(), hdc, 0);

	return DrawThemeBackground(hTheme, hdc, iPartId, iStateId, &rect, clipRect);
}

void Window::OpenTheme() {
	if(!hTheme)
		hTheme = OpenThemeData(getWindowHandle(), GetThemeApplicableClassList().c_str());
}

void Window::setOnClose(WndEventBase *onClose) {
	this->onClose = onClose;
}

void Window::setOnDestroy(WndEventBase *onDestroy) {
	onDestroy_ = onDestroy;
}

void Window::setOnPaintWindow(WndEventBase *onPaintWindow) {
	this->onPaintWindow = onPaintWindow;
}

void Window::setOnShowWindow(WndEventBase *onShowWindow) {
	this->onShowWindow = onShowWindow;
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

bool Window::WMEraseBackground(HDC hdc) {
	RECT rc = {0, 0, getWidth(), getHeight()};
	FillRect(hdc, &rc, (HBRUSH) COLOR_WINDOW);
	return false;
}

long int Window::GetThemeFontInt(int iPartId, int iStateId, LOGFONT& logFont) {
	OpenTheme();
	return ::GetThemeFont(getTheme(), 0, iPartId, iStateId, TMT_FONT, &logFont);
}

HFONT Window::GetThemeFont(int iPartId, int iStateId) {
	LOGFONT logFont;
	if(GetThemeFontInt(iPartId, iStateId, logFont))
		return (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	return CreateFontIndirect(&logFont);
}

void Window::setFont(HFONT font) {
	SendMessage(*this, WM_SETFONT,
			(WPARAM) font, 1);
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

	wndClassRegistered = (RegisterClassEx(&wcex) != 0);
}

LRESULT __stdcall Window::IntWndProc(HWND wnd, UINT msg, WPARAM wParam,
		LPARAM lParam) {
	Window *pwnd = static_cast<Window*>((void*) GetWindowLongPtr(wnd,
			GWLP_USERDATA));
	try {
		if ((!pwnd || wnd != pwnd->getWindowHandle()) && !wndCreating)
			DestroyWindow(wnd); //the window has a wrong pointer. KILL IT! ;)
	} catch (...) {
		if (wndCreating)
			return DefWindowProc(wnd, msg, wParam, lParam); //OK
		// Seems we've got something wrong... force
		// the window destruction
		DestroyWindow(wnd);
	}
	if (wndCreating)
		return DefWindowProc(wnd, msg, wParam, lParam); //OK
	return pwnd->WndProc(msg, wParam, lParam);
}

BOOL Window::Show() {
	BOOL res = ShowWindow(getWindowHandle(), SW_SHOW);
	ShowWindowInt();
	onShowWindow(*this);
	return res;
}

BOOL Window::Hide() {
	return ShowWindow(getWindowHandle(), SW_HIDE);
}

BOOL Window::Show(int type) {
	BOOL res = ShowWindow(getWindowHandle(), type);
	ShowWindowInt();
	onShowWindow(*this);
	return res;
}

BOOL Window::MoveWindow() {
	return SetWindowPos(0, SWP_NOACTIVATE);
}
BOOL Window::SetWindowPos(HWND insertAfter, UINT flags) {
	return ::SetWindowPos(getWindowHandle(), insertAfter, getX(), getY(),
			getWidth(), getHeight(), flags);
}
BOOL Window::SetWindowPos(HWND insertAfter, int x, int y, int width, int height,
		UINT flags) {
	this->x_ = x;
	this->y_ = y;
	this->width_ = width;
	this->height_ = height;
	return SetWindowPos(insertAfter, flags);
}

BOOL Window::MoveWindow(int x, int y, int width, int height) {
	this->x_ = x;
	this->y_ = y;
	this->width_ = width;
	this->height_ = height;
	return MoveWindow();
}

BOOL Window::setName(const wstring& name) {
	return SetWindowText(getWindowHandle(), name.c_str());
}

wstring Window::getName() const {
	size_t txtLength = SendMessageW(getWindowHandle(), WM_GETTEXTLENGTH, 0, 0) + 1;
	wchar_t *buff = new wchar_t[txtLength];
	GetWindowTextW(getWindowHandle(), buff, txtLength);
	wstring text(buff);
	delete buff;
	return text;
}

BOOL Window::setSize(int width, int height) {
	this->width_ = width;
	this->height_ = height;
	return MoveWindow();
}

BOOL Window::setPosition(int x, int y) {
	this->x_ = x;
	this->y_ = y;
	return MoveWindow();
}

BOOL Window::setMenu(HMENU hMenu) {
	return ::SetMenu(getWindowHandle(), hMenu);
}

HMENU Window::getMenu() const {
	return ::GetMenu(getWindowHandle());
}

LONG_PTR Window::setStyle(LONG_PTR style) {
	LONG_PTR res = SetWindowLongPtr(getWindowHandle(), GWL_STYLE,
			getStyle() | style);
	MoveWindow(); //Setting style does not change anything before we actually move window
	return res;
}

LONG_PTR Window::getStyle() const {
	return GetWindowLongPtr(getWindowHandle(), GWL_STYLE);
}

LONG_PTR Window::clearStyle(LONG_PTR clearBits) {
	LONG_PTR res = SetWindowLongPtr(getWindowHandle(), GWL_STYLE,
			getStyle() & ~clearBits);
	SetWindowPos(0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	// Setting style does not change anything before we actually move window
	return res;
}

LONG_PTR Window::clearStyleEx(LONG_PTR clearBits) {
	LONG_PTR res = SetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE,
			getStyleEx() & ~clearBits);
	SetWindowPos(0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	// Setting style does not change anything before we actually move window
	return res;
}

LONG_PTR Window::setStyleEx(LONG_PTR styleEx) {
	LONG_PTR res = SetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE,
			getStyleEx() | styleEx);
	SetWindowPos(0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); // see Window::SetStyle
	return res;
}

LONG_PTR Window::getStyleEx() const {
	return GetWindowLongPtr(getWindowHandle(), GWL_EXSTYLE);
}

HWND Window::setParent(HWND parentWindow) {
	HWND res = ::SetParent(getWindowHandle(), parentWindow);
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
	InvalidateRect(getWindowHandle(), updateRect, 1);
}

void Window::ImmediatelyUpdateWindow() {
	UpdateWindow();
	::UpdateWindow(getWindowHandle());
}

Window *Window::SafeGetWindow(UINT wCmd) {
	HWND wnd = GetWindow(getWindowHandle(), wCmd);
	if (!wnd)
		return 0;
	return SafeWindowFromHandle(wnd);
}

void Window::setPaintCachingMode(bool cacheOn) {
	cacheOn_ = cacheOn;
}

void Window::setRedraw(bool redraw) {
	SendMessage(getWindowHandle(), WM_SETREDRAW, redraw, 0);
}
