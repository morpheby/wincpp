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
#include <Window.h>

const std::vector<WidgetEventType> wndRegisteredSimpleMessages = {
		WidgetEventType::themeChange,
		WidgetEventType::setFocus,
		WidgetEventType::killFocus,
		(WidgetEventType) WM_ACTIVATE,
		WidgetEventType::close
};

const std::set<WidgetEventType> wndInternalMessages = {
		WidgetEventType::destroy,
		WidgetEventType::geometryChange,
		WidgetEventType::setFocus,
		WidgetEventType::killFocus,
		WidgetEventType::themeChange,
		WidgetEventType::showStateChange,
		WidgetEventType::styleChange,
		WidgetEventType::close,
		WidgetEventType::drawWidget,
		(WidgetEventType) WM_ACTIVATE
};

Widget::~Widget() {
	deleting_ = true;
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
	x_{x}, y_{y}, width_{width}, height_{height},
	widthOuter_{width}, heightOuter_{height} {
}

Widget::Widget(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style, Widget& parent) :
	window_{nullptr},
	windowName_{name}, style_{style},
	showState_{false},
	x_{x}, y_{y}, width_{width}, height_{height},
	widthOuter_{width}, heightOuter_{height},
	parent_{parent.getShared()} {

	// Window will be loaded as soon as it is necessary - initially it is not shown
}

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
	window_ = std::unique_ptr<Window> (
			new Window(windowName_, (DWORD) style_, x_, y_, widthOuter_, heightOuter_,
			( !parent_.expired() ? parent_.lock()->getWindow().getWindowHandle() : 0), 0, 0)
	);
	if(!window_)
		return false;

	reloadSize();
	reloadPosition();
	setInternalMessages();
	setExternalMessages();
	InitWindow();

	if(showState_)
		Show();

	return true;
}

void Widget::reloadSize() {
	width_ = getWindow().getSize().cx;
	height_ = getWindow().getSize().cy;
	RECT wndRect = getWindow().getWindowRect();
	widthOuter_ = wndRect.right-wndRect.left;
	heightOuter_ = wndRect.bottom-wndRect.top;
}

void Widget::reloadPosition() {
	x_ = getWindow().getPosition().x;
	y_ = getWindow().getPosition().y;
}

void Widget::setInternalMessages() {
	for(auto msg : wndRegisteredSimpleMessages) {
		getWindow().setProcessMessage((UINT) msg, NewEventExt(*this, &Widget::wndMessage));
	}
	getWindow().setProcessMessage((UINT) WidgetEventType::styleChange, NewEventExt(*this, &Widget::wndStyleChange));
	getWindow().setProcessMessage((UINT) WidgetEventType::showStateChange, NewEventExt(*this, &Widget::wndShowStateChange));
	getWindow().setProcessMessage((UINT) WidgetEventType::destroy, NewEventExt(*this, &Widget::wndDestroy));
	getWindow().setProcessMessage(WM_SIZE, NewEventExt(*this, &Widget::wndGeomChange));
	getWindow().setProcessMessage(WM_MOVE, NewEventExt(*this, &Widget::wndGeomChange));
	getWindow().setPainter(NewEventExt(*this, &Widget::wndDrawWindow));
}

void Widget::setExternalMessages() {
	for(auto msg : msgMap_) {
		if(!wndInternalMessages.count(msg.first))
			getWindow().setProcessMessage((UINT) msg.first, NewEventExt(*this, &Widget::wndMessage));
	}
}

void Widget::DrawWindow(Drawing::Drawer &drawer) {
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

int Widget::recycleEvent(WidgetEventType event, WidgetEventParams &params) {
	switch(event) {
	default:
		msgMap_[event](*this, params);
		break;
	}
	return 0;
}

void Widget::UpdateChildren() {
	for(auto i : attachedWidgets_) {

	}
}

void Widget::Show() {
	getWindow().Show();
}

void Widget::setEventHandler(WidgetEventType event,
		WidgetEventExtBase<WidgetEventParams&>* handler) {
	if(handler) {
		msgMap_[event] = handler;
		if(!wndInternalMessages.count(event))
			getWindow().setProcessMessage((UINT) event, NewEventExt(*this, &Widget::wndMessage));
	} else
		msgMap_.erase(event);
}

int Widget::recycleEvent(WidgetEventType event) {
	WidgetEventParams params {event};
	recycleEvent(event, params);
}

void Widget::reloadStyle() {
	style_ = (WidgetStyle) getWindow().getStyle();
}

void Widget::reloadGeometry() {
	reloadPosition();
	reloadSize();
}

void Widget::reloadShowState() {
	// Do nothing - job is done in wndShowStateChange
}

int Widget::wndDrawWindow(Window& sender, Drawing::Drawer &drawer) {
	DrawWindow(drawer);
	WidgetDrawEventParams params {WidgetEventType::drawWidget, drawer};
	recycleEvent(WidgetEventType::drawWidget, params);
	return 1;
}

int Widget::wndDestroy(Window &wnd, WinMessage_t &msg) {
	if(!deleting_) {
		LoadWindow(); // reload window
		recycleEvent(WidgetEventType::destroy);
	}
	return 0;
}

int Widget::wndGeomChange(Window &wnd, WinMessage_t &msg) {
	reloadGeometry();
	recycleEvent(WidgetEventType::geometryChange);
	return 1;
}

int Widget::wndStyleChange(Window& wnd, WinMessage_t& msg) {
	reloadStyle();
	recycleEvent(WidgetEventType::styleChange);
	return 1;
}

int Widget::wndShowStateChange(Window& wnd, WinMessage_t& msg) {
	showState_ = msg.wParam ? SW_SHOW : SW_HIDE;
	reloadShowState();
	recycleEvent(WidgetEventType::showStateChange);
	return 1;
}

int Widget::wndMessage(Window& wnd, WinMessage_t& msg) {
	switch ((WidgetEventType) msg.msg) {
	default:
		break;
	}
	WidgetWinMsgParams params {(WidgetEventType) msg.msg, msg};
	msg.retVal = recycleEvent((WidgetEventType)msg.msg, params);
	return 0;
}
