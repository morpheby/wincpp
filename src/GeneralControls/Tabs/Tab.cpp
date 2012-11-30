/*
 * Tab.cpp
 *
 *  Created on: 30.11.2012
 *      Author: Morphe
 */

#include "Tab.h"

#include <cassert>

namespace Tabs {

Tab::Tab() {
	// Check if we are convertible to Widget
	assert(dynamic_cast<Widget*> ((Widget*)this) != nullptr);

	controller_ = SharePtr(new TabController());
}

Tab::~Tab() {
}

Widget* Tab::getThisWidget() {
	return (Widget*) this;
}

void Tab::changeController(std::shared_ptr<TabController> newController) {
	// If we were the only tab, holding that controller - it will be deleted
	controller_ = newController;
}

} /* namespace Tabs */
