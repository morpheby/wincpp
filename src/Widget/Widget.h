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
	geometryChange  = WM_USER + 1,
	setFocus		= WM_SETFOCUS,
	killFocus		= WM_KILLFOCUS,
	themeChange		= WM_THEMECHANGED,
	showStateChange	= WM_SHOWWINDOW,
	styleChange		= WM_STYLECHANGED,
	close			= WM_CLOSE,
	drawWidget		= WM_PAINT
};

template <typename... _styles>
constexpr WidgetStyle wsCombine(WidgetStyle style, _styles... styles) {
	return (WidgetStyle) ( ( (DWORD) style ) | ( (DWORD) wsCombine(styles...) ) );
}

template<>
constexpr WidgetStyle wsCombine(WidgetStyle style) {
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
	inline int getWidth() const {return width_;}
	inline int getHeight() const {return height_;}

	void setPosition(int x, int y);
	inline void setX(int x) {setPosition(x, getY());}
	inline void setY(int y) {setPosition(getX(), y);}
	inline int getX() const {return x_;}
	inline int getY() const {return y_;}

	inline int getRight() const {return getX()+getWidth();}
	inline int getBottom() const {return getY()+getHeight();}

	std::shared_ptr<Widget> getShared();

	template<class _ChildTp>
	std::shared_ptr<_ChildTp> getShared();

	std::weak_ptr<Widget> setParent(std::weak_ptr<Widget> parent);

	void Show();

	void setEventHandler(WidgetEventType event, WidgetEventExtBase<WidgetEventParams&> *handler);
protected:
	void KillWindow();
	bool LoadWindow(); // return true on success

	virtual void DrawWindow(Drawing::Drawer &drawer);

	Window & getWindow();
private:
	/* Platform-dependent members */
	std::unique_ptr<Window> window_; // to allow Window reload
	std::map<WidgetEventType, WidgetEventExtCaller<WidgetEventParams&> > msgMap_;

	/* Platform-independent members */
	std::set<std::shared_ptr<Widget>,
				std::owner_less<std::shared_ptr<Widget>>> attachedWidgets_;
	std::wstring windowName_;
	WidgetStyle style_;
	bool showState_;
	int x_, y_, width_, height_, widthOuter_, heightOuter_;
	std::weak_ptr<Widget> parent_;

	bool deleting_ = false;

	void reloadSize();
	void reloadPosition();
	void reloadStyle();
	void reloadShowState();
	void reloadGeometry();
	void setInternalMessages();
	void setExternalMessages();
	void InitWindow();
	void UpdateChildren();

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

	int wndMessage(Window &wnd, WinMessage_t &msg); // Pass all registered events
};

template<class _ChildTp>
inline std::shared_ptr<_ChildTp> Widget::getShared() {
	std::dynamic_pointer_cast<_ChildTp>(getShared());
}

#endif /* WIDGET_H_ */
