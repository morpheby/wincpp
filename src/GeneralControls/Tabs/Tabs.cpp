/*
 * Tabs.cpp
 *
 *  Created on: 04.11.2012
 *      Author: Morphe
 */

#include "Tabs.h"
#include <ThemedDrawer.h>

Tabs::Tabs() :
		Widget(L"Tab Group", 100_scaled, 100_scaled, 500_scaled, 500_scaled, getWindowDefaultStyle()) {
	setEventHandler(WidgetEventType::childAttached, NewEventExt(*this, &Tabs::onChildAttached));
	setEventHandler(WidgetEventType::childDetach, NewEventExt(*this, &Tabs::onChildDetached));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &Tabs::onGeometryChange));
}

Tabs::~Tabs() {
}

int Tabs::onChildAttached(Widget& sender, WidgetEventParams& _params) {
	WidgetToWidgetEventParams& params = static_cast<WidgetToWidgetEventParams&> (_params);
	params.refWidget.setSize(getWidth(), getHeight()-30_scaled);
	params.refWidget.setPosition(0, 30_scaled);
	params.refWidget.Hide();
	UpdateTabButtons();
	params.refWidget.setStyle(wsCombine(WidgetStyle::isChild, WidgetStyle::borderThin));
	return 0;
}

int Tabs::onChildDetached(Widget& sender, WidgetEventParams& _params) {
	WidgetToWidgetEventParams& params = static_cast<WidgetToWidgetEventParams&> (_params);
	params.refWidget.Show();
	UpdateTabButtons();
	params.refWidget.setStyle(getWindowDefaultStyle());
	return 0;
}

int Tabs::onBtnClicked(Window& sender) {
	setName(sender.getName());
	if(selection_) {
		selection_->setSelected(false);
		selection_->getControlledWidget()->Hide();
	}
	selection_ = dynamic_cast<TabButton&>(sender).getShared();
	selection_->setSelected(true);
	selection_->getControlledWidget()->Show();
}

int Tabs::onGeometryChange(Widget& sender, WidgetEventParams& params) {
	for(auto tab : getAttachedWidgets()) {
		tab->setSize(getWidth(), getHeight()-30_scaled);
	}
}

void Tabs::UpdateTabButtons() {
	tabs_.clear();
	int xPosition = 5_scaled;
	for(auto tab : getAttachedWidgets()) {
		tabs_.push_back(SharePtr(new TabButton(tab, xPosition, 5_scaled, 25_scaled, getWindow())));
		tabs_.back()->setOnClick(NewEvent(*this, &Tabs::onBtnClicked));
		xPosition += tabs_.back()->getSizeX() + 5_scaled;
	}
}

Tabs::TabButton::TabButton(std::shared_ptr<Widget> controlledWidget, int x, int y, int height, HWND parent) :
		ButtonWnd(ButtonWnd::DefPushButton, controlledWidget->getName(), x, y, 0, height, parent),
		selected_{false},
		controlledWidget_{controlledWidget} {
	RECT txtRect;
	memset(&txtRect, 0, sizeof(txtRect));
	auto drawerPtr = getDrawer();
	if(auto tDrawer = std::dynamic_pointer_cast<Drawing::ThemedDrawer>(drawerPtr))
		tDrawer->setFontThemed(TEXT_LABEL, 0, TMT_FONT);
	drawerPtr->drawText(controlledWidget->getName(), DT_CALCRECT, txtRect);
	drawerPtr->clearFont();
	setSizeX(txtRect.right + 10_scaled);
}

void Tabs::TabButton::PaintWindow(Drawing::Drawer& drawer) {
	if(isSelected()) {
	}
}
