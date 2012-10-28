/* Window.h
 * Provides basic front-end for Win32 windows.
 * Please, avoid it's direct use. Use Widget class.
 *
 * This class is used directly only in special cases,
 * such as double-layering CommonControls - there is an
 * Window object, which by fact is just an wrapper of
 * some default window class, and an Widget, which acts
 * as real object and contains that Window as its private
 * inaccessible child
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdlib.h>
#include <map>

#include <windows.h>

#include <uxtheme.h>

#include <string>

#include <Events.h>
#include "vssym32.h"
#include "Bitmap.h"
#include "DPIScaler.h"
#include "DeviceContext.h"
#include "Drawer.h"

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
	WndEventExtCaller() { event_ = std::shared_ptr<EventGenBase>(new WndEventExtBase<_ParamT>()); }
	int operator() (Window &sentBy, _ParamT param) {
		return (*dynamic_cast<WndEventExtBase<_ParamT> *>(event_.get()))(sentBy, param);
	}
	using EventCallerBase::operator=;
};

template<class _ParentT, class _ParamT>
WndEventExtBase<_ParamT> * NewEventExt
	(_ParentT &parent, int(_ParentT::*f)(Window &w, _ParamT param)) {
	return new WndEventExt<_ParentT, _ParamT> (parent, f);
}

struct WinMessage_t {
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
	LRESULT retVal;
};

class Window {
public:
	Window(void);
	Window(const std::wstring& wndName, DWORD style, int x, int y, int width, int height,
			HWND parentWindow, HMENU menu, LPVOID lpParam);
	virtual ~Window(void);

	BOOL SetWindowPos(HWND insertAfter, int x, int y, int width, int height,
			UINT flags);

	BOOL Show();
	BOOL Hide();
	BOOL Show(int type);

	void UpdateWindow();
	void UpdateWindow(RECT *updateRect);
	void ImmediatelyUpdateWindow();

	BOOL setName(const std::wstring& name);
	std::wstring getName() const;

	BOOL setSize(int width, int height);
	BOOL setSize(SIZE sz);
	BOOL setPosition(int x, int y);
	BOOL setPosition(POINT pos);
	SIZE getSize() const;
	POINT getPosition() const;
	RECT getWindowRect() const;
	RECT getClientRect() const;

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
	static Window* SafeWindowFromHandle(const HWND wnd);

	void setFont(HFONT font);
	void setPaintCachingMode(bool cacheOn);

	DC::DeviceContext getDC();

	std::shared_ptr<Drawing::Drawer> getDrawer();

	void setPainter(WndEventExtBase<Drawing::Drawer&> *painter);
	void setProcessMessage(UINT msg, WndEventExtBase<WinMessage_t&> *msgProc);

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
	void InitVars();

	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void PaintWindow(Drawing::Drawer &drawer) {
	}

	virtual void PrePaintWindow(LPRECT updateRect) {
	}

	virtual bool WMEraseBackground(DC::DeviceContext dc);

	static LRESULT __stdcall IntWndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual std::wstring GetThemeApplicableClassList();
	virtual LRESULT DefWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual HWND CreateWnd(const std::wstring& wndName, UINT style,
			int x, int y, int width, int height, HWND parentWnd, HMENU menu,
			HINSTANCE instance, LPVOID lpParam);
//	HPEN CreateCosmeticPen(COLORREF color);
	void OpenTheme();
//	long int GetThemeFontInt(int iPartId, int iStateId, LOGFONT& logFont);
	void setDefMsgProcessing();
	void resetDefMsgProcessing();
	void setRedraw(bool redraw);

private:
	HWND hWnd;
	Bitmap* cachedBmp;
	bool update_, defMsgProc, cacheOn_;
	HTHEME hTheme_;
	static bool wndClassRegistered;
	static unsigned int wndCreating;

	void IntPaintWindow();

	bool NeedsUpdate() {
		return update_ ? (update_ = false), true : false;
	}

	static void RegisterWndClass();
	static bool CheckScreenResolution(int width, int height);

	void IntCachedPaint(DC::DeviceContext dc, RECT updateRect);

	std::map<DWORD, WndEventExtCaller<WinMessage_t&>> msgMap_;
	WndEventExtCaller<Drawing::Drawer&> painter_;

};

#endif /* WINDOW_H_ */
