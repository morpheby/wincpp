/*
 * Widget.cpp
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "Widget.h"
#include "Window.h"

Widget::~Widget() {
}

Widget::Widget() :
	Widget(WidgetStyle::none) {
}

Widget::Widget(Widget& parent) :
	Widget(WidgetStyle::none, parent) {
}

Widget::Widget(WidgetStyle style) :
	Widget(L"", style) {
}

Widget::Widget(WidgetStyle style, Widget& parent) :
	Widget(L"", style, parent) {
}

Widget::Widget(const std::wstring& name) :
	Widget(name, WidgetStyle::none) {
}

Widget::Widget(const std::wstring& name, Widget& parent) :
	Widget(name, WidgetStyle::none, parent) {
}

Widget::Widget(const std::wstring& name, WidgetStyle style) :
	Widget(name, 0, 0, 0, 0, style) {
}

Widget::Widget(const std::wstring& name, WidgetStyle style, Widget& parent) :
	Widget(name, 0, 0, 0, 0, style, parent) {
}

Widget::Widget(int x, int y, int width, int height) :
	Widget(x, y, width, height, WidgetStyle::none) {
}

Widget::Widget(int x, int y, int width, int height, Widget& parent) :
	Widget(x, y, width, height, WidgetStyle::none, parent) {
}

Widget::Widget(int x, int y, int width, int height, WidgetStyle style) :
	Widget(L"", x, y, width, height, WidgetStyle::none) {
}

Widget::Widget(int x, int y, int width, int height, WidgetStyle style,
		Widget& parent) :
	Widget(L"", x, y, width, height, WidgetStyle::none, parent) {
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height) :
	Widget(name, x, y, width, height, WidgetStyle::none) {
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		Widget& parent) :
	Widget(name, x, y, width, height, WidgetStyle::none, parent) {
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style) {
	// Real implementation
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style, Widget& parent) {
	// Real implementation
}

//
//LRESULT Widget::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
//	int processed = false;
//	int retVal = 0;
//	HRGN updRgn = 0;
//
//	switch (msg) {
//	case WM_ERASEBKGND:
//		if(cacheOn_) {
//			processed = true;
//			retVal = 1;
//		}
//		break;
//	case WM_MOVE:
//		ReloadPos();
//		ReloadSize(); // If movement occured together with sizing,
//					  // than our size data may become outdated
//		WMMove();
//		break;
//	case WM_SIZE:
//		ReloadPos();
//		ReloadSize();
//		update_ = true;
//		WMSize();
//		break;
//	case WM_SHOWWINDOW:
//		// XXX update state
//		onShowWindow(*this);
//		break;
//	case WM_THEMECHANGED:
//		CloseThemeData(hTheme);
//		OpenTheme();
//		break;
//	case WM_STYLECHANGED:
//		// XXX update state
//		break;
//	default:
//		break;
//	}
//
//	return processed ? retVal : DefWndProc(msg, wParam, lParam);
//}
