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
#include <errorParse.h>
#include <vector>
#include <utility>
// XXX include only on Win32
#include "../Window/Window.h"

const std::vector<WidgetEventType> wndRegisteredMessages = {
	WidgetEventType::styleChange,
	WidgetEventType::sizeChange,
	WidgetEventType::posChange,
	WidgetEventType::themeChange,
	WidgetEventType::setFocus,
	WidgetEventType::killFocus,
	(WidgetEventType) WM_ACTIVATE,
	WidgetEventType::showStateChange,
	WidgetEventType::destroy
};

Widget::~Widget() {
}

Widget::Widget() :
	Widget(WidgetStyle::none) {}
Widget::Widget(Widget& parent) :
	Widget(WidgetStyle::none, parent) {}
Widget::Widget(WidgetStyle style) :
	Widget(L"", style) {}
Widget::Widget(WidgetStyle style, Widget& parent) :
	Widget(L"", style, parent) {}
Widget::Widget(const std::wstring& name) :
	Widget(name, WidgetStyle::none) {}
Widget::Widget(const std::wstring& name, Widget& parent) :
	Widget(name, WidgetStyle::none, parent) {}
Widget::Widget(const std::wstring& name, WidgetStyle style) :
	Widget(name, 0, 0, 0, 0, style) {}
Widget::Widget(const std::wstring& name, WidgetStyle style, Widget& parent) :
	Widget(name, 0, 0, 0, 0, style, parent) {}
Widget::Widget(int x, int y, int width, int height) :
	Widget(x, y, width, height, WidgetStyle::none) {}
Widget::Widget(int x, int y, int width, int height, Widget& parent) :
	Widget(x, y, width, height, WidgetStyle::none, parent) {}
Widget::Widget(int x, int y, int width, int height, WidgetStyle style) :
	Widget(L"", x, y, width, height, WidgetStyle::none) {}
Widget::Widget(int x, int y, int width, int height, WidgetStyle style,
		Widget& parent) :
	Widget(L"", x, y, width, height, WidgetStyle::none, parent) {}
Widget::Widget(const std::wstring& name, int x, int y, int width, int height) :
	Widget(name, x, y, width, height, WidgetStyle::none) {}
Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		Widget& parent) :
	Widget(name, x, y, width, height, WidgetStyle::none, parent) {}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style) :
	windowName_{name}, style_{style},
	showState_{false},
	x_{x}, y_{y}, width_{width}, height_{height} {
	LoadWindow();
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style, Widget& parent) :
	window_{nullptr},
	windowName_{name}, style_{style},
	showState_{false},
	x_{x}, y_{y}, width_{width}, height_{height},
	parent_{parent.getShared()} {

	// Window will be loaded as soon as it is necessary - initially it is not shown
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

void Widget::setSize(int width, int height) {
	getWindow().setSize(width, height);
}

void Widget::setPosition(int x, int y) {
	getWindow().setPosition(x, y);
}

void Widget::KillWindow() {
	window_ = nullptr;
}

bool Widget::LoadWindow() {
	if(window_)
		KillWindow();
	window_ = std::unique_ptr<Window> (
			new Window(windowName_, (DWORD) style_, x_, y_, width_, height_,
			( !parent_.expired() ? parent_.lock()->getWindow().getWindowHandle() : 0), 0, 0)
	);
	if(!window_)
		return false;

	LoadSize();
	LoadPosition();
	setInternalMessages();
	setExternalMessages();

	if(showState_)
		Show();

	return true;
}

void Widget::LoadSize() {
	width_ = getWindow().getSize().cx;
	height_ = getWindow().getSize().cy;
}

void Widget::LoadPosition() {
	x_ = getWindow().getPosition().x;
	y_ = getWindow().getPosition().y;
}

void Widget::setInternalMessages() {
	for(auto msg : wndRegisteredMessages) {
		getWindow().setProcessMessage((UINT) msg, NewEventExt(*this, &Widget::wndMessage));
	}
	getWindow().setPainter(NewEventExt(*this, &Widget::wndDrawWindow));
}

void Widget::setExternalMessages() {
}

void Widget::DrawWindow(DC::DeviceContext dc) {
}

Window& Widget::getWindow() {
	if( !window_ &&			// No valid window present
		!LoadWindow()) {	// and LoadWindow failed
		ShowAppError(
				L"Unable to restore window.\n"
				 "Application will now terminate",
				L"Critical error", MB_OK|MB_ICONERROR);
		exit(GetLastError());
	}

	return *window_;
}

void Widget::InitWindow() {
	getWindow().setPaintCachingMode(true); // all widgets shall use cached paint
}

std::shared_ptr<Widget> Widget::getShared() {
	return shared_from_this();
}

void Widget::attachChild(Widget &child) {
	attachedWidgets_.insert(child.getShared());
}

void Widget::detachChild(Widget &child) {
	attachedWidgets_.erase(child.getShared());
}

int Widget::wndDrawWindow(Window& sender, DC::DeviceContext dc) {
	DrawWindow(dc);
	return 1;
}

int Widget::wndDestroy() {
	if(!deleting_)
		LoadWindow(); // reload window
	return 0;
}

int Widget::wndGeomChange() {
	return 1;
}

int Widget::recycleEvent(WidgetEventType msg) {
	// Pass the event to all next registered parties
	return 0; // XXX
}

void Widget::UpdateChildren() {
	for(auto i : attachedWidgets_) {

	}
}

void Widget::Show() {
	getWindow().Show();
}

int Widget::wndMessage(Window& wnd, struct WinMessage_t& msg) {
	msg.retVal = recycleEvent((WidgetEventType) msg.msg);
	switch ((WidgetEventType) msg.msg) {
		case WidgetEventType::destroy:
			wndDestroy();
			break;
		case WidgetEventType::styleChange:
			style_ = (WidgetStyle) getWindow().getStyle();
			break;
		case WidgetEventType::showStateChange:
			showState_ = msg.wParam ? SW_SHOW : SW_HIDE;
			break;
		default:
			break;
	}
	return 1;
}
