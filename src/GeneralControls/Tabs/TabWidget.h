/*
 * Tab.h
 *
 *  Created on: 30.11.2012
 *      Author: Morphe
 */

#ifndef TAB_H_
#define TAB_H_

namespace Tabs {
class TabWidget;
}

#include <Widget.h>
#include "TabController.h"

namespace Tabs {

class TabWidget : public Widget {
	friend class Tabs::TabController;
public:
	~TabWidget();
	using Widget::Widget;
private:
	std::shared_ptr<TabController> controller_;

	void changeController(std::shared_ptr<TabController> newController);
};

} /* namespace Tabs */

#endif /* TAB_H_ */
