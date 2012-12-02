/*
 * Widget.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include <platform.h>
#include <set>
#include <map>
#include <string>
#include <Drawer.h>
#include <Events.h>
#include <DPIScaler.h>

#include "WidgetEventParams.h"
#include <vector>

class Window;
class Widget;
typedef struct _tagWINMESSAGE WinMessage_t;

// XXX separate platform-dependent declarations
enum class WidgetStyle : DWORD {
	none			= 0,
	hasTitle		= WS_CAPTION,
	hasMinimizeBox	= WS_MINIMIZEBOX,
	hasMaximizeBox	= WS_MAXIMIZEBOX,
	hasSysMenu		= WS_SYSMENU,
	borderThin		= WS_BORDER,
	borderThick		= WS_THICKFRAME,
	borderDialog	= WS_DLGFRAME,
	isChild			= WS_CHILD
};

enum class WidgetEventType : UINT {
	destroy			= WM_DESTROY,
	geometryChange  = WM_WINDOWPOSCHANGED,
	setFocus		= WM_SETFOCUS,
	killFocus		= WM_KILLFOCUS,
	themeChange		= WM_THEMECHANGED,
	showStateChange	= WM_SHOWWINDOW,
	styleChange		= WM_STYLECHANGED,
	close			= WM_CLOSE,
	drawWidget		= WM_PAINT,
	mouseMove		= WM_MOUSEMOVE,
	mouseLBtnDown	= WM_LBUTTONDOWN,
	mouseRBtnDown	= WM_RBUTTONDOWN,
	mouseMBtnDown	= WM_MBUTTONDOWN,
	mouseLBtnUp		= WM_LBUTTONUP,
	mouseRBtnUp		= WM_RBUTTONUP,
	mouseMBtnUp		= WM_MBUTTONUP,
	mouseLBtnDClick	= WM_LBUTTONDBLCLK,
	mouseRBtnDClick	= WM_RBUTTONDBLCLK,
	mouseMBtnDClick	= WM_MBUTTONDBLCLK,
	__internalEvent	= WM_USER + 1,	// don't allow overlap between window
	childAttached,					// and widget events
	childDetach,
	parentChanged
};

template <typename... _styles>
constexpr WidgetStyle wsCombine(WidgetStyle style, _styles... styles) {
	return (WidgetStyle) ( ( (DWORD) style ) | ( (DWORD) wsCombine(styles...) ) );
}

template<>
constexpr WidgetStyle wsCombine(WidgetStyle style) {
	return style;
}

template <typename... _styles>
constexpr WidgetStyle wsExclude(WidgetStyle style, _styles... styles) {
	return (WidgetStyle) ( ( (DWORD) style ) & (~ (DWORD) wsCombine(styles...) ) );
}

template<>
constexpr WidgetStyle wsExclude(WidgetStyle style) {
	return style;
}

constexpr WidgetStyle getWindowDefaultStyle() {
	return wsCombine(WidgetStyle::hasTitle, WidgetStyle::hasMinimizeBox,
						WidgetStyle::hasMaximizeBox, WidgetStyle::hasSysMenu,
						WidgetStyle::borderThick);
}


typedef EventBase<Widget> WidgetEventBase;
typedef EventCaller<Widget> WidgetEventCaller;

template<typename _ParamT>
class WidgetEventExtBase : public EventGenBase {
public:
	WidgetEventExtBase() {}
	~WidgetEventExtBase() {}
	virtual int operator()(Widget &sentBy, _ParamT param) {return 0;}
};

template<class _ParentT, class _ParamT>
class WidgetEventExt : public WidgetEventExtBase<_ParamT> {
private:
	typedef int (_ParentT::*_FuncPtr_t)(Widget &o, _ParamT param);
	_FuncPtr_t f_;
	_ParentT &parent;
public:
	WidgetEventExt(_ParentT &x, _FuncPtr_t f) : f_(f), parent(x) {}
	int operator() (Widget &sender, _ParamT param) {
		return (parent.*f_)(sender, param);
	}
};

template<typename _ParamT>
class WidgetEventExtCaller : protected EventCallerBase {
public:
	WidgetEventExtCaller() { event_ = std::shared_ptr<EventGenBase>(new WidgetEventExtBase<_ParamT>()); }
	int operator() (Widget &sentBy, _ParamT param) {
		return (*dynamic_cast<WidgetEventExtBase<_ParamT> *>(event_.get()))(sentBy, param);
	}
	using EventCallerBase::operator=;
};

template<class _ParentT, class _ParamT>
WidgetEventExtBase<_ParamT> * NewEventExt
	(_ParentT &parent, int(_ParentT::*f)(Widget &w, _ParamT param)) {
	return new WidgetEventExt<_ParentT, _ParamT> (parent, f);
}

/*
 * Widget
 * NOTE: Always store Widget objects in shared_ptr.
 * 		 When creating child widgets, consider following
 * 		 syntax:
 * 		 	weak_ptr<Widget> child = (new Widget(*mainWidget))->getShared();
 * 		 		OR
 * 		 	shahred_ptr<Widget> child (new Widget(*mainWidget));
 */
class Widget : public std::enable_shared_from_this<Widget> {
public:
	Widget();
	Widget(Widget &parent);
	Widget(WidgetStyle style);
	Widget(WidgetStyle style, Widget &parent);
	Widget(const std::wstring &name);
	Widget(const std::wstring &name, Widget &parent);
	Widget(const std::wstring &name, WidgetStyle style);
	Widget(const std::wstring &name, WidgetStyle style, Widget &parent);
	Widget(int x, int y, int width, int height);
	Widget(int x, int y, int width, int height, Widget &parent);
	Widget(int x, int y, int width, int height, WidgetStyle style);
	Widget(int x, int y, int width, int height, WidgetStyle style, Widget &parent);
	Widget(const std::wstring &name, int x, int y, int width, int height);
	Widget(const std::wstring &name, int x, int y, int width, int height,
			Widget &parent);
	Widget(const std::wstring &name, int x, int y, int width, int height,
			WidgetStyle style);
	Widget(const std::wstring &name, int x, int y, int width, int height,
			WidgetStyle style, Widget &parent);

	Widget(const Widget &w) = delete;
	Widget(Widget &&w) = delete;

	virtual ~Widget();


	void setSize(int width, int height);
	inline void setWidth(int width) {setSize(width, getHeight());}
	inline void setHeight(int height) {setSize(getWidth(), height);}
	int getWidth() const;
	int getHeight() const;

	void setPosition(int x, int y);
	inline void setX(int x) {setPosition(x, getY());}
	inline void setY(int y) {setPosition(getX(), y);}
	int getX() const;
	int getY() const;

	inline int getRight() const {return getX()+getWidth();}
	inline int getBottom() const {return getY()+getHeight();}

	std::shared_ptr<Widget> getShared();

	template<class _ChildTp>
	std::shared_ptr<_ChildTp> getShared();

	std::shared_ptr<Widget> getParent();
	std::weak_ptr<Widget> setParent(std::weak_ptr<Widget> parent);
	std::weak_ptr<Widget> setParent(std::nullptr_t) {
		return setParent(std::shared_ptr<Widget> ());
	}

	void Show();
	void Hide();

	void setEventHandler(WidgetEventType event, WidgetEventExtBase<WidgetEventParams&> *handler);
	void setOnWidgetReload(WidgetEventBase *handler);

	void setName(const std::wstring &name);

	const std::vector<std::shared_ptr<Widget>>& getAttachedWidgets() const {
		return attachedWidgets_;
	}

	const std::wstring& getName() const {
		return windowName_;
	}

	WidgetStyle setStyle(WidgetStyle newStyle);
	WidgetStyle getStyle();

	bool isSelfHoldEnabled() const {
		return selfHoldEnabled_;
	}

	void setSelfHoldEnabled(bool selfHoldEnabled) {
		selfHoldEnabled_ = selfHoldEnabled;
	}

protected:
	void KillWindow();
	bool LoadWindow(); // return true on success

	virtual void DrawWindow(Drawing::Drawer &drawer);

	Window & getWindow();
	Window & getWindowConst() const;

	virtual void widgetReload();
private:
	/* Platform-dependent members */
	std::unique_ptr<Window> window_; // to allow Window reload
	std::map<WidgetEventType, WidgetEventExtCaller<WidgetEventParams&> > msgMap_;
	WidgetEventCaller widgetReload_;

	/* Platform-independent members */
	std::vector<std::shared_ptr<Widget>> attachedWidgets_;
	std::wstring windowName_;
	WidgetStyle style_;
	bool visible_;
	int showState_;	// showState_: -1 = Minimizes / 0 = Normal / 1 = Maximized
	int x_, y_, width_, height_, widthOuter_, heightOuter_;
	std::weak_ptr<Widget> parent_;

	bool deleting_ = false, selfHoldEnabled_ = false;

	void reloadSize();
	void reloadPosition();
	void reloadStyle();
	void reloadShowState();
	void reloadGeometry();
	void setInternalMessages();
	void setExternalMessages();
	void InitWindow();

	void attachChild(Widget &child);
	void detachChild(Widget &child);


	/* Internal events */
	int wndDrawWindow(Window &sender, Drawing::Drawer &drawer); // Call DrawWindow

	int wndDestroy(Window &wnd, WinMessage_t &msg); // If we are not destroying the window, recreate it
	int wndGeomChange(Window &wnd, WinMessage_t &msg); // Updates position and size
	int wndStyleChange(Window &wnd, WinMessage_t &msg);
	int wndShowStateChange(Window &wnd, WinMessage_t &msg);

	int recycleEvent(WidgetEventType event);
	int recycleEvent(WidgetEventType event, WidgetEventParams &params); // Passes all registered events
	int recycleEvent(WidgetEventType event, WidgetEventParams &&params);

	int wndMessage(Window &wnd, WinMessage_t &msg); // Pass all registered events
};

template<class _ChildTp>
inline std::shared_ptr<_ChildTp> Widget::getShared() {
	std::dynamic_pointer_cast<_ChildTp>(getShared());
}

constexpr bool operator & (const WidgetStyle &__lsv, const WidgetStyle &__rsv) {
	return ((UINT) __lsv) & ((UINT) __rsv);
}

#endif /* WIDGET_H_ */
