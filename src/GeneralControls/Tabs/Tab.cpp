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
	assert((Widget*) this != nullptr);
}

Tab::~Tab() {
}

Widget* Tab::getThisWidget() {
	return dynamic_cast<Widget*> (this);
}

} /* namespace Tabs */
