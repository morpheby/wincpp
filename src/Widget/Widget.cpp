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
#include <algorithm>

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
		WidgetEventType::mouseMove,
		(WidgetEventType) WM_ACTIVATE
};

Widget::~Widget() {
	deleting_ = true;
	if(window_)
		getWindow().clearMessageMap();
}

Widget::Widget() :
			window_{nullptr},
			windowName_{L""}, style_{WidgetStyle::none},
			visible_{false}, showState_{0},
			x_{0}, y_{0}, width_{0}, height_{0},
			widthOuter_{0}, heightOuter_{0} {
}

// Widget::Widget(Widget &&w) :
//             window_{w.window_},
//             msgMap_{w.msgMap_},
//             widgetReload_{w.widgetReload_},
//             attachedWidgets_{w.attachedWidgets_},
//             windowName_{w.windowName_},
//             style_{w.style_},
//             showState_{w.showState_},
//             visible_{w.visible_},
//             x_{w.x_}, y_{w.y_}, width_{w.width_}, height_{w.height_},
//             widthOuter_{w.widthOuter_},
//             heightOuter_{w.heightOuter_},
//             parent_{w.parent_},
//             deleting_{w.deleting_},
//             selfHoldEnabled_{w.selfHoldEnabled_} {
// }

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
	widgetReload();

	if(visible_)
		Show();
	else
		Hide();

	return true;
}

void Widget::reloadSize() {
	if(showState_ != 0)
		return; // Keep prevoius values
	width_ = getWindow().getSize().cx;
	height_ = getWindow().getSize().cy;
	RECT wndRect = getWindow().getWindowRect();
	widthOuter_ = wndRect.right-wndRect.left;
	heightOuter_ = wndRect.bottom-wndRect.top;
}

void Widget::reloadPosition() {
	if(showState_ != 0)
		return; // Keep prevoius values
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
	getWindow().setProcessMessage((UINT) WidgetEventType::geometryChange, NewEventExt(*this, &Widget::wndGeomChange));
	getWindow().setProcessMessage((UINT) WidgetEventType::mouseMove, NewEventExt(*this, &Widget::wndMouseMove));
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
	attachedWidgets_.push_back(child.getShared());
	child.setStyle(WidgetStyle::isChild);
	recycleEvent(WidgetEventType::childAttached, WidgetToWidgetEventParams{WidgetEventType::childAttached, child});
}

void Widget::detachChild(Widget &child) {
	child.setStyle(getWindowDefaultStyle());
	recycleEvent(WidgetEventType::childDetach, WidgetToWidgetEventParams{WidgetEventType::childDetach, child});
	attachedWidgets_.erase(std::find(attachedWidgets_.begin(), attachedWidgets_.end(), child.getShared()));
}

std::weak_ptr<Widget> Widget::setParent(std::weak_ptr<Widget> parent) {
	std::weak_ptr<Widget> oldParent = parent_;
	if(!oldParent.expired())
		oldParent.lock()->detachChild(*this);
	parent_ = parent;
	if(!parent_.expired()) {
		auto parentPtr = parent_.lock();
		parentPtr->attachChild(*this);
		if(window_)
			getWindow().setParent(parentPtr->getWindow());
	} else
		if(window_)
			LoadWindow();
//		getWindow().setParent(0);

	recycleEvent(WidgetEventType::parentChanged);

	return oldParent;
}

int Widget::recycleEvent(WidgetEventType event, WidgetEventParams &params) {
	// While we are recycling event, ensure we don't disappear...
	std::shared_ptr<Widget> t;
	if(selfHoldEnabled_)
		t = getShared();
	switch(event) {
	default:
		return msgMap_[event](*this, params);
	}
	return 0;
}

int Widget::recycleEvent(WidgetEventType event, WidgetEventParams &&params) {
	return recycleEvent(event, params);
}

int Widget::recycleEvent(WidgetEventType event) {
	recycleEvent(event, WidgetEventParams{event});
}

void Widget::Show() {
	visible_ = true;
	if(showState_ == 0)
		getWindow().Show(SW_SHOWNORMAL);
	else if(showState_ < 0)
		getWindow().Show(SW_SHOWMINIMIZED);
	else
		getWindow().Show(SW_SHOWMAXIMIZED);
}

void Widget::Hide() {
	visible_ = false;
	if(window_)
		getWindow().Hide();
}

void Widget::setEventHandler(WidgetEventType event,
		WidgetEventExtBase<WidgetEventParams&>* handler) {
	if(handler) {
		msgMap_[event] = handler;
		if(!wndInternalMessages.count(event) && window_)
			getWindow().setProcessMessage((UINT) event, NewEventExt(*this, &Widget::wndMessage));
	} else
		msgMap_.erase(event);
}

void Widget::reloadStyle() {
	style_ = (WidgetStyle) getWindow().getStyle();
}

void Widget::reloadGeometry() {
	reloadShowState();
	reloadPosition();
	reloadSize();
}

void Widget::reloadShowState() {
//	visible_ = IsWindowVisible(getWindow());
	if(IsIconic(getWindow()))
		showState_ = -1;
	else if(IsZoomed(getWindow()))
		showState_ = 1;
	else
		showState_ = 0;
}

int Widget::wndDrawWindow(Window& sender, Drawing::Drawer &drawer) {
	DrawWindow(drawer);
	recycleEvent(WidgetEventType::drawWidget, WidgetDrawEventParams{WidgetEventType::drawWidget, drawer});
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
	UINT flags = ((WINDOWPOS*)msg.lParam)->flags;
//	if(flags & SWP_HIDEWINDOW)
//		visible_ = false;
//	if(flags & SWP_SHOWWINDOW)
//		visible_ = true;
	// NOTE: state of visiblity shall be controlled ONLY with
	// internal class API, since i.e. DestroyWindow first sends
	// SWP_HIDEWINOW before destruction
	if((flags & SWP_NOSIZE) && (flags & SWP_NOMOVE))
		return 0;
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
//	visible_ = msg.wParam;
	reloadShowState();
	recycleEvent(WidgetEventType::showStateChange);
	return 1;
}

int Widget::wndMouseMove(Window& wnd, WinMessage_t& msg) {
	POINT mousePos;
	GetCursorPos(&mousePos);
	recycleEvent(WidgetEventType::mouseMove, WidgetMouseEventParams{WidgetEventType::mouseMove, mousePos});
	return 1;
}

void Widget::setOnWidgetReload(WidgetEventBase* handler) {
	widgetReload_ = handler;
}

void Widget::setName(const std::wstring &name) {
	windowName_ = name;
	if(window_)
			getWindow().setName(name);
}

WidgetStyle Widget::setStyle(WidgetStyle newStyle) {
	WidgetStyle oldStyle = style_;
	style_ = newStyle;
	if(window_)
		getWindow().setStyle((LONG_PTR)style_);
	return oldStyle;
}

WidgetStyle Widget::getStyle() {
	return style_;
}

int Widget::getWidth() const {
	if(showState_ > 0 && window_)
		return getWindowConst().getSizeX();
	else
		return width_;
}

int Widget::getHeight() const {
	if(showState_ > 0 && window_)
		return getWindowConst().getSizeY();
	else
		return height_;
}

int Widget::getX() const {
	if(showState_ > 0 && window_)
		return getWindowConst().getPositionX();
	else
		return x_;
}

int Widget::getY() const {
	if(showState_ > 0 && window_)
		return getWindowConst().getPositionY();
	else
		return y_;
}

Window& Widget::getWindowConst() const {
	if( !window_) {
		ShowAppError(
				L"No window present",
				L"Critical error", MB_OK|MB_ICONERROR);
		exit(GetLastError());
	}
	return *window_;
}

std::shared_ptr<Widget> Widget::getParent() {
	if(!parent_.expired())
		return parent_.lock();
	else
		return nullptr;
}

void Widget::widgetReload() {
	widgetReload_(*this);
}

int Widget::wndMessage(Window& wnd, WinMessage_t& msg) {
	switch ((WidgetEventType) msg.msg) {
	default:
		break;
	}
	WidgetWinMsgParams params {(WidgetEventType) msg.msg, msg};
	msg.retVal = recycleEvent((WidgetEventType)msg.msg, params);
	return params.final;
}

int Widget::getHeightOuter() const {
	return heightOuter_;
}

int Widget::getWidthOuter() const {
	return widthOuter_;
}

void Widget::ReattachDeserializedChildsEvent() {
	std::vector<std::shared_ptr<Widget>> widgetsToReattach;
	std::swap(widgetsToReattach, attachedWidgets_);
	if(widgetsToReattach.empty())
		return;   // XXX Actually, reloading window is not a good idea in case it is a
	              // XXX child window -- size and position may reset.
	LoadWindow(); // Prepare window without children
	for(auto w : widgetsToReattach)
		w->setParent(getShared());
}

void Widget::UpdateStateDeserializedEvent() {
//	LoadWindow();
}

void Widget::RegisterFields() {
	RegisterField(windowName_);
	RegisterField(x_);
	RegisterField(y_);
	RegisterField(width_);
	RegisterField(height_);
	RegisterField(style_);
	RegisterField(visible_);
	RegisterField(showState_);
	RegisterField(widthOuter_);
	RegisterField(heightOuter_, std::bind(&Widget::UpdateStateDeserializedEvent, std::ref(*this)));
	// RegisterField(msgMap_); // wtf? how would you map realtime objects??
	RegisterField(attachedWidgets_, std::bind(&Widget::ReattachDeserializedChildsEvent, std::ref(*this)));
}
