/*
 * TabController.cpp
 *
 *  Created on: 23.11.2012
 *      Author: Morphe
 */

#include "TabButton.h"
#include "TabController.h"
#include <ThemedDrawer.h>
#include <algorithm>


namespace Tabs {

void TabController::init() {
	setEventHandler(WidgetEventType::childAttached, NewEventExt(*this, &TabController::onChildAttached));
	setEventHandler(WidgetEventType::childDetach, NewEventExt(*this, &TabController::onChildDetach));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &TabController::onGeometryChange));
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
	WidgetToWidgetEventParams& params = static_cast<WidgetToWidgetEventParams&> (_params);
	addTabBtn(params.refWidget.getName());
	params.refWidget.setSize(getWidth(), getHeight() - tabs_.back()->getSizeY());
	params.refWidget.setPosition(0, tabs_.back()->getSizeY());
	params.refWidget.Hide();
	params.refWidget.setStyle(wsCombine(WidgetStyle::isChild));
	dynamic_cast<TabWidget&> (params.refWidget).changeController(this->getShared<TabController>());
	if(!selection_)
		selectTab(tabs_.back());
	return 0;
}

int TabController::onChildDetach(Widget& sender, WidgetEventParams& params) {
}

int TabController::onGeometryChange(Widget& sender, WidgetEventParams& params) {
}

int TabController::onBtnClicked(Window& sender, WinMessage_t& msg) {
	selectTab(dynamic_cast<TabButton&> (sender).shared_from_this());
}

void TabController::addTabBtn(const std::wstring& name) {
	int lastXPos = tabs_.empty() ? 0 : tabs_.back()->getPositionX() + tabs_.back()->getSizeX();
	tabs_.push_back(SharePtr(new TabButton(name, lastXPos, 0, getWindow())));
	tabs_.back()->setProcessMessage(WM_LBUTTONDOWN, NewEventExt(*this, &TabController::onBtnClicked));
}

void TabController::selectTab(const std::shared_ptr<TabButton> &tab) {
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

void TabController::UpdateTabButtons() {
	tabs_.clear();
	for(auto tab : getAttachedWidgets()) {
		addTabBtn(tab->getName());
	}
	selectTab(tabs_.back());
}

} /* namespace Tabs */
