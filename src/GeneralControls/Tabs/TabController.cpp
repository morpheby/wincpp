/*
 * TabController.cpp
 *
 *  Created on: 23.11.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "TabButton.h"
#include "TabController.h"
#include <ThemedDrawer.h>
#include <algorithm>
#include <cstdlib>


namespace Tabs {

class TabPool {
public:
	TabPool() = default;
	virtual ~TabPool() = default;

	void registerController(std::shared_ptr<TabController> controller);
	std::shared_ptr<TabController> findControllerAt(int x, int y);
private:
	std::vector<std::weak_ptr<TabController>> controllers_;
};

TabPool TabController::tabPool_ {};

void TabPool::registerController(std::shared_ptr<TabController> controller) {
	controllers_.push_back(controller);
}

std::shared_ptr<TabController> TabPool::findControllerAt(int x, int y) {
	auto i = controllers_.begin();
	while(i != controllers_.end()) {
		if(i->expired()) {	// cleanup
			controllers_.erase(i);
			continue;
		}
		auto p = i->lock();
		if(	p->getX() <= x && x <= p->getRight() &&
			p->getY() <= y && y <= p->getBottom() )
			return p;
		++i;
	}
	return nullptr;
}

void TabController::init() {
	origName_ = getName();
	setEventHandler(WidgetEventType::childAttached, NewEventExt(*this, &TabController::onChildAttached));
	setEventHandler(WidgetEventType::childDetach, NewEventExt(*this, &TabController::onChildDetach));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &TabController::onGeometryChange));
	setEventHandler(WidgetEventType::mouseMove, NewEventExt(*this, &TabController::onMouseMove));
	setEventHandler(WidgetEventType::mouseLBtnUp, NewEventExt(*this, &TabController::onMouseLBtnUp));
	setEventHandler(WidgetEventType::setFocus, NewEventExt(*this, &TabController::onSetFocus));
}

TabController::TabController() :
	Widget(WidgetStyle::none) {init();}
TabController::TabController(Widget& parent) :
	Widget(WidgetStyle::none, parent) {init();}
TabController::TabController(WidgetStyle style) :
	Widget(L"", style) {init();}
TabController::TabController(WidgetStyle style, Widget& parent) :
	Widget(L"", style, parent) {init();}
TabController::TabController(const std::wstring& name) :
	Widget(name, WidgetStyle::none) {init();}
TabController::TabController(const std::wstring& name, Widget& parent) :
	Widget(name, WidgetStyle::none, parent) {init();}
TabController::TabController(const std::wstring& name, WidgetStyle style) :
	Widget(name, 0, 0, 0, 0, style) {init();}
TabController::TabController(const std::wstring& name, WidgetStyle style, Widget& parent) :
	Widget(name, 0, 0, 0, 0, style, parent) {init();}
TabController::TabController(int x, int y, int width, int height) :
	Widget(x, y, width, height, WidgetStyle::none) {init();}
TabController::TabController(int x, int y, int width, int height, Widget& parent) :
	Widget(x, y, width, height, WidgetStyle::none, parent) {init();}
TabController::TabController(int x, int y, int width, int height, WidgetStyle style) :
	Widget(L"", x, y, width, height, style) {init();}
TabController::TabController(int x, int y, int width, int height, WidgetStyle style,
		Widget& parent) :
	Widget(L"", x, y, width, height, style, parent) {init();}
TabController::TabController(const std::wstring& name, int x, int y, int width, int height) :
	Widget(name, x, y, width, height, WidgetStyle::none) {init();}
TabController::TabController(const std::wstring& name, int x, int y, int width, int height,
		Widget& parent) :
	Widget(name, x, y, width, height, WidgetStyle::none, parent) {init();}
TabController::TabController(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style) :
	Widget(name, x, y, width, height, style) {init();}
TabController::TabController(const std::wstring& name, int x, int y, int width, int height,
		WidgetStyle style, Widget& parent) :
	Widget(name, x, y, width, height, style, parent) {init();}

TabController::~TabController() {
}

int TabController::onChildAttached(Widget& sender, WidgetEventParams& _params) {
	if(!registered_) {
		tabPool_.registerController(getShared<TabController>());
		registered_ = true;
	}
	WidgetToWidgetEventParams& params = static_cast<WidgetToWidgetEventParams&> (_params);
	addTabBtn(params.refWidget.getName());
	params.refWidget.setSize(getWidth(), getHeight() - tabs_.back()->getSizeY());
	params.refWidget.setPosition(0, tabs_.back()->getSizeY());
	params.refWidget.Hide();
	dynamic_cast<TabWidget&> (params.refWidget).changeController(this->getShared<TabController>());
	selectTab(tabs_.back());
	getWindow().UpdateWindow();
	return 0;
}

int TabController::onChildDetach(Widget& sender, WidgetEventParams& _params) {
	WidgetToWidgetEventParams& params = static_cast<WidgetToWidgetEventParams&> (_params);
	dynamic_cast<TabWidget&> (params.refWidget).changeController(nullptr);
	getWindow().UpdateWindow();
	return 0;
}

int TabController::onGeometryChange(Widget& sender, WidgetEventParams& params) {
	if(tabs_.empty())
		return 0;
	for(auto tab : getAttachedWidgets()) {
		tab->setSize(getWidth(), getHeight() - tabs_.back()->getSizeY());
	}
	return 1;
}

int TabController::onSetFocus(Widget& sender, WidgetEventParams& params) {
	if(selection_)
		::SetFocus(selection_->getWindow());
	return 0;
}

int TabController::onBtnClicked(Window& sender, WinMessage_t& msg) {
	selectTab(dynamic_cast<TabButton&> (sender).shared_from_this());
	GetCursorPos(&mouseMovePos_);
	btnMouseDown_ = true;
	return 0;
}

int TabController::onCloseBtnClicked(Window& sender) {
	auto tabBtn = dynamic_cast<TabButton&> (sender).shared_from_this();
	auto tabP = std::find(tabs_.begin(), tabs_.end(), tabBtn);
	auto tabW = std::dynamic_pointer_cast<TabWidget> (*(getAttachedWidgets().begin() + (tabP - tabs_.begin())));
	tabW->Hide();
	removeTab(tabBtn);
	tabW->setParent(nullptr);
	return 0;
}

void TabController::addTabBtn(const std::wstring& name) {
	int lastXPos = tabs_.empty() ? 0 : tabs_.back()->getPositionX() + tabs_.back()->getSizeX();
	tabs_.push_back(std::make_shared<TabButton>(name, lastXPos, 0, getWindow()));
	tabs_.back()->setProcessMessage(WM_LBUTTONDOWN, NewEventExt(*this, &TabController::onBtnClicked));
	tabs_.back()->setProcessMessage(WM_LBUTTONUP, NewEventExt(*this, &TabController::onBtnMouseReleased));
	tabs_.back()->setProcessMessage(WM_MOUSELEAVE, NewEventExt(*this, &TabController::onBtnMouseReleased));
	tabs_.back()->setProcessMessage(WM_MOUSEMOVE, NewEventExt(*this, &TabController::onBtnMouseMove));
	tabs_.back()->setOnCloseClick(NewEvent(*this, &TabController::onCloseBtnClicked));
}

void TabController::selectTab(std::shared_ptr<TabButton> tab) {
	if(selectionBtn_ == tab)
		return;
	if(selectionBtn_)
		selectionBtn_->setActive(false);
	selectionBtn_ = tab;
	selectionBtn_->setActive(true);

	auto tabP = std::find(tabs_.begin(), tabs_.end(), selectionBtn_);

	if(selection_)
		selection_->Hide();

	selection_ = std::dynamic_pointer_cast<TabWidget> (*(getAttachedWidgets().begin() + (tabP - tabs_.begin())));

	setName(selection_->getName());
	selection_->Show();
	::SetFocus(selection_->getWindow());
}

void TabController::removeTab(std::shared_ptr<TabButton> tab) {
	auto tabP = std::find(tabs_.begin(), tabs_.end(), tab);

	if(selectionBtn_ == tab)
		if(tabs_.size() == 1) {
			selectionBtn_ = nullptr;
			selection_ = nullptr;
			setName(origName_);
			getWindow().UpdateWindow(); // When all tab buttons are removed, remove bar
		} else if(tabP + 1 == tabs_.end())
			selectTab(*(tabP - 1));
		else
			selectTab(*(tabP + 1));

	int xPos = tab->getPositionX();
	tabs_.erase(tabP);

	for(auto i = tabP; i != tabs_.end(); ++i) {
		(*i)->setPositionX(xPos);
		xPos += (*i)->getSizeX();
	}
}

void TabController::DrawWindow(Drawing::Drawer& drawer) {
	if(tabs_.empty())
		return;
	if(Drawing::ThemedDrawer *tDrawer = dynamic_cast<Drawing::ThemedDrawer*>(&drawer)) {
		// Actually, TabItem doesn't have font, but do we care? ))
		RECT rect = {0, 0, getWidth(), tabs_.back()->getSizeY()};
		tDrawer->drawThemedEdges(rect, TABP_TABITEM, TIS_NORMAL, BDR_RAISEDOUTER | BDR_SUNKENINNER, BF_BOTTOM);
	}
}

void TabController::widgetReload() {
	UpdateTabButtons();
}

int TabController::onBtnMouseMove(Window& sender, WinMessage_t& msg) {
	if(btnMouseDown_) {
		POINT pos;
		GetCursorPos(&pos);
		if(	std::abs(pos.x - mouseMovePos_.x) <= 2 &&
			std::abs(pos.y - mouseMovePos_.y) <= 2	)
			return 0;
		// User is moving tab, start capture
		mouseCaptured_ = true;
		btnMouseDown_ = false;
		firstInversion_ = true;
		SetCapture(getWindow());
	}
	return 0;
}

int TabController::onBtnMouseReleased(Window& sender, WinMessage_t& msg) {
	btnMouseDown_ = false;
	return 0;
}

void TabController::drawSquareInv(std::shared_ptr<Drawing::Drawer> drawerPtr) {
	drawerPtr->drawRectColorInverted(mouseMovePos_.x, mouseMovePos_.y, 5,
			getHeightOuter());
	drawerPtr->drawRectColorInverted(mouseMovePos_.x, mouseMovePos_.y,
			getWidthOuter(), 5);
	drawerPtr->drawRectColorInverted(mouseMovePos_.x,
			mouseMovePos_.y + getHeightOuter() - 5, getWidthOuter(), 5);
	drawerPtr->drawRectColorInverted(mouseMovePos_.x + getWidthOuter() - 5,
			mouseMovePos_.y, 5, getHeightOuter());
}

int TabController::onMouseMove(Widget& sender, WidgetEventParams& params_) {
	WidgetMouseEventParams &params = static_cast<WidgetMouseEventParams&> (params_);
	if(!mouseCaptured_)
		return 0;

	// Draw rectangle on desktop to show what we are moving
	auto drawerPtr = getWindow().getDesktopDrawer();

	if(!firstInversion_)
		drawSquareInv(drawerPtr);
	else
		firstInversion_ = false;
	mouseMovePos_ = params.mousePoint_;
	drawSquareInv(drawerPtr);

	return 0;
}

int TabController::onMouseLBtnUp(Widget& sender, WidgetEventParams& params) {
	if(mouseCaptured_) {
		POINT pos;
		ReleaseCapture();
		mouseCaptured_ = false;
		auto drawerPtr = getWindow().getDesktopDrawer();
		drawSquareInv(drawerPtr);
		GetCursorPos(&pos);
		std::shared_ptr<TabController> c;
		auto tab = selection_;
		if(!(c = tabPool_.findControllerAt(pos.x, pos.y))) {
			c = buildController(pos.x, pos.y, getWidthOuter(), getHeightOuter());
			c->setSelfHoldEnabled(true);
			c->Show();
			c->setEventHandler(WidgetEventType::close, NewEventExt(*c, &TabController::onCloseInternal));
		}
		if(c == getShared())
			return 0;
		tab->Hide();
		removeTab(selectionBtn_);
		tab->setParent(c);
	}
	return 0;
}

int TabController::onCloseInternal(Widget& sender, WidgetEventParams& params) {
	std::vector<std::shared_ptr<Widget>> tmp = getAttachedWidgets();
	for(auto i : tmp) {
		i->Hide();
		i->setParent(nullptr);
	}
	return 1;
}

void TabController::UpdateTabButtons() {
	tabs_.clear();
	for(auto tab : getAttachedWidgets()) {
		addTabBtn(tab->getName());
	}
	if(!tabs_.empty())
		selectTab(tabs_.back());
}

} /* namespace Tabs */
