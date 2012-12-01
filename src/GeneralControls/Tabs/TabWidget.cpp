/*
 * Tab.cpp
 *
 *  Created on: 30.11.2012
 *      Author: Morphe
 */

#include "TabWidget.h"
#include <cassert>

namespace Tabs {

TabWidget::~TabWidget() {
}

void TabWidget::changeController(std::shared_ptr<TabController> newController) {
	// If we were the only tab, holding that controller - it will be deleted
	controller_ = newController;
}

} /* namespace Tabs */
