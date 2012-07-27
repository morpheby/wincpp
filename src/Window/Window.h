#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdlib.h>

#include <windows.h>

#include <uxtheme.h>

#include <string>

#include <Events.h>
#include "vssym32.h"
#include "Bitmap.h"
#include "DPIScaler.h"

class Window;

typedef EventBase<Window> WndEventBase;
typedef EventCaller<Window> WndEventCaller;

template<typename _ParamT>
class WndEventExtBase : public EventGenBase {
public:
	WndEventExtBase() {}
	~WndEventExtBase() {}
	virtual int operator()(Window &sentBy, _ParamT param) {return 0;}
};

template<class _ParentT, class _ParamT>
class WndEventExt : public WndEventExtBase<_ParamT> {
private:
	typedef int (_ParentT::*_FuncPtr_t)(Window &o, _ParamT param);
	_FuncPtr_t f_;
	_ParentT &parent;
public:
	WndEventExt(_ParentT &x, _FuncPtr_t f) : f_(f), parent(x) {}
	int operator() (Window &sender, _ParamT param) {
		return (parent.*f_)(sender, param);
	}
};

template<typename _ParamT>
class WndEventExtCaller : protected EventCallerBase {
public:
	WndEventExtCaller() { event_ = new WndEventExtBase<_ParamT>(); }
	int operator() (Window &sentBy, _ParamT param) {
		return (*dynamic_cast<WndEventExtBase<_ParamT> *>(event_))(sentBy, param);
	}
	using EventCallerBase::operator=;
};

template<class _ParentT, class _ParamT>
WndEventExtBase<_ParamT> * NewEventExt
	(_ParentT &parent, int(_ParentT::*f)(Window &w, _ParamT param)) {
	return new WndEventExt<_ParentT, _ParamT> (parent, f);
}

class Window {
public:
	Window(void);
	Window(const std::wstring& wndName, DWORD style, int x, int y, int width, int height,
			HWND parentWindow, HMENU menu, LPVOID lpParam);
	virtual ~Window(void);

	BOOL MoveWindow(int x, int y, int width, int height);
	BOOL SetWindowPos(HWND insertAfter, int x, int y, int width, int height,
			UINT flags);
	BOOL SetWindowPos(HWND insertAfter, UINT flags);
	BOOL MoveWindow();

	BOOL Show();
	BOOL Hide();
	BOOL Show(int type);

	void UpdateWindow();
	void UpdateWindow(RECT *updateRect);
	void ImmediatelyUpdateWindow();

	BOOL setName(const std::wstring& name);
	std::wstring getName() const;

	BOOL setSize(int width, int height);

	inline int getWidth() const {
		return width_;
	}

	inline int getHeight() const {
		return height_;
	}

	BOOL SetPosition(int x, int y);

	inline int getX() const {
		return x_;
	}

	inline int getY() const {
		return y_;
	}

	inline HTHEME getTheme() const {
		return hTheme;
	}

	BOOL setMenu(HMENU menu);
	HMENU getMenu() const;

	LONG_PTR setStyle(LONG_PTR style);
	LONG_PTR clearStyle(LONG_PTR clearBits);
	LONG_PTR getStyle() const;

	LONG_PTR setStyleEx(LONG_PTR styleEx);
	LONG_PTR clearStyleEx(LONG_PTR clearBits);
	LONG_PTR getStyleEx() const;

	HWND setParent(HWND parentWindow);
	HWND getParent() const;

	HWND getWindowHandle() const {
		return hWnd;
	}

	operator HWND() const {
		return getWindowHandle();
	}

	Window *SafeGetWindow(UINT wCmd);
	void setOnClose(WndEventBase* onClose);
	void setOnDestroy(WndEventBase* onDestroy);
	static Window* SafeWindowFromHandle(const HWND wnd);
	void setOnPaintWindow(WndEventBase* onPaintWindow);
	void setOnShowWindow(WndEventBase* onShowWindow);

	void setOnSetFocus(WndEventBase* onSetFocus) {
		this->onSetFocus = onSetFocus;
	}

	void setOnKillFocus(WndEventBase* onKillFocus) {
		this->onKillFocus = onKillFocus;
	}

	void setFont(HFONT font);

	enum _WMDlgKeys {
		WM_TAB = WM_USER,
		WM_ESC = WM_USER + 1,
		WM_ENTER = WM_USER + 2
	};

protected:
	Window(bool tagDelayCreate);
	void PostWindowCreate(const std::wstring& wndName, DWORD style, int x, int y, int width,
			int height, HWND parentWindow, HMENU menu, LPVOID lpParam);

	void AssignClass();
	void ReloadSize();
	void InitVars(int x, int y, int width, int height);

	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void PaintWindow(HDC hdc) {
	}

	virtual void PrePaintWindow(LPRECT updateRect) {
	}

	WndEventCaller onShowWindow;

	virtual void ShowWindowInt() {
	}

	virtual void WMSize() {
	}

	virtual void WMMove() {
	}

	virtual bool WMSetFocus() {
		return false;
	}

	virtual bool WMKillFocus() {
		return false;
	}

	virtual void WMKeyDown(int vk) {
	}

	virtual void WMChar(wchar_t ch) {
	}

	virtual bool WMEraseBackground(HDC hdc);
	WndEventCaller onPaintWindow;
	WndEventCaller onClose, onDestroy_;
	WndEventCaller onSetFocus, onKillFocus;

	virtual bool WMClose() {
		return false;
	}

	virtual void WMDestroy() {
	}

	static LRESULT __stdcall IntWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual std::wstring GetThemeApplicableClassList();
	virtual LRESULT DefWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual HWND CreateWnd(const std::wstring& wndName, UINT style, HWND parentWnd,
			HMENU menu, HINSTANCE instance, LPVOID lpParam);
	HRESULT DrawTText(HDC hdc, int iPartId, int iStateId, const std::wstring& text,
			DWORD dwFlags, RECT& rect);
	HRESULT DrawTText(HDC hdc, const std::wstring& text, DWORD dwFlags, RECT& rect);
	HRESULT DrawTBackground(HDC hdc, int iPartId, int iStateId, RECT& rect);
	HRESULT DrawTBackground(HDC hdc, int iPartId, int iStateId, RECT& rect,
			RECT* clipRect);
	HFONT GetThemeFont(int iPartId, int iStateId);
	HPEN CreateCosmeticPen(COLORREF color);
	void OpenTheme();
	long int GetThemeFontInt(int iPartId, int iStateId, LOGFONT& logFont);
	void setDefMsgProcessing();
	void resetDefMsgProcessing();
	void setPaintCachingMode(bool cacheOn);
	void setRedraw(bool redraw);
	void ReloadPos();

private:
	HWND hWnd;
	HTHEME hTheme;
	int x_, y_, width_, height_;
	Bitmap* cachedBmp;
	bool update_, defMsgProc, cacheOn_;
	static bool wndClassRegistered;
	static unsigned int wndCreating;

	void IntPaintWindow();

	bool NeedsUpdate() {
		return update_ ? (update_ = false), true : false;
	}

	static void RegisterWndClass();
	static bool CheckScreenResolution(int width, int height);

	void IntCachedPaint(HDC dc, RECT updateRect);

};

#endif /* WINDOW_H_ */
