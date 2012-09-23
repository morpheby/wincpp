/*
 * Widget.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include <platform.h>
#include <DeviceContext.h>
#include <set>
#include <string>

class Window;

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
	sizeChange		= WM_SIZE,
	posChange		= WM_MOVE,
	setFocus		= WM_SETFOCUS,
	killFocus		= WM_KILLFOCUS,
	themeChange		= WM_THEMECHANGED,
	showStateChange	= WM_SHOWWINDOW,
	styleChange		= WM_STYLECHANGED
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

/*
 * Widget
 * NOTE: Always store Widget objects in shared_ptr.
 * 		 When creating child widgets, consider following
 * 		 syntax:
 * 		 	weak_ptr<Widget> child = (new Widget(*mainWidget))->getShared();
 * 		 		OR
 * 		 	shahred_ptr<Widget> child = (new Widget(*mainWidget))->getShared();
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
protected:
	void KillWindow();
	bool LoadWindow(); // return true on success

	virtual void DrawWindow(DC::DeviceContext dc);

	Window & getWindow();
private:
	/* Platform-dependent members */
	std::unique_ptr<Window> window_; // to allow Window reload
//	std::map<UINT, >

	/* Platform-independent members */
	std::set<std::shared_ptr<Widget>,
				std::owner_less<std::shared_ptr<Widget>>> attachedWidgets_;
	std::wstring windowName_;
	WidgetStyle style_;
	bool showState_;
	int x_, y_, width_, height_;
	std::weak_ptr<Widget> parent_;

	bool deleting_ = false;

	void LoadSize();
	void LoadPosition();
	void setInternalMessages();
	void setExternalMessages();
	void InitWindow();
	void UpdateChildren();

	void attachChild(Widget &child);
	void detachChild(Widget &child);


	/* Internal events */
	int wndDrawWindow(Window &sender, DC::DeviceContext dc); // Call DrawWindow

	int wndDestroy(); // If we are not destroying the window, recreate it
	int wndGeomChange(); // Updates position and size

	int recycleEvent(WidgetEventType msg); // Passes all registered events

	int wndMessage(Window &wnd, struct WinMessage_t &msg); // Pass all registered events
};

template<class _ChildTp>
inline std::shared_ptr<_ChildTp> Widget::getShared() {
	std::dynamic_pointer_cast<_ChildTp>(getShared());
}

#endif /* WIDGET_H_ */
