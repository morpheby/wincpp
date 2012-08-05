/*
 * Widget.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include <windows.h>

class Window;

enum class WidgetStyle {
	none			= 0,
	hasTitle		= WS_CAPTION,
	hasMinimizeBox	= WS_MINIMIZEBOX,
	hasMaximizeBox	= WS_MAXIMIZEBOX,
	hasSysMenu		= WS_SYSMENU,
	borderThin		= WS_BORDER,
	borderThick		= WS_THICKFRAME,
	borderDialog	= WS_DLGFRAME
};

class Widget {
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

	~Widget();


	int setSize(int width, int height);
	inline void setWidth(int width) {setSize(width, getHeight());}
	inline void setHeight(int height) {setSize(getWidth(), height);}
	inline int getWidth() const {return width_;}
	inline int getHeight() const {return height_;}

	int setPosition(int x, int y);
	inline void setX(int x) {setPosition(x, getY());}
	inline void setY(int y) {setPosition(getX(), y);}
	inline int getX() const {return x_;}
	inline int getY() const {return y_;}

	inline int getRight() const {return getX()+getWidth();}
	inline int getBottom() const {return getY()+getHeight();}


protected:

private:
	std::set<Widget&> attachedWidgets_;
	Window *window_; // to allow Window reload
	std::wstring windowName_;
	DWORD style_;
	HTHEME hTheme;
	int x_, y_, width_, height_;

	void LoadSize();
	void LoadPosition();
};

#endif /* WIDGET_H_ */
