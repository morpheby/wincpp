/*
 * Tab.h
 *
 *  Created on: 30.11.2012
 *      Author: Morphe
 */

#ifndef TAB_H_
#define TAB_H_

namespace Tabs {
class Tab;
}

#include <Widget.h>
#include "TabController.h"

namespace Tabs {

class Tab {
	friend class Tabs::TabController;
public:
	Tab();
	~Tab();
private:
	std::shared_ptr<TabController> controller_;

	Widget *getThisWidget();
	void changeController(std::shared_ptr<TabController> newController);
};

} /* namespace Tabs */
#endif /* TAB_H_ */
