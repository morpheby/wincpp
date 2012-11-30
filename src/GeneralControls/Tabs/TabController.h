/*
 * TabController.h
 *
 *  Created on: 23.11.2012
 *      Author: Morphe
 */

#ifndef TABCONTROLLER_H_
#define TABCONTROLLER_H_

namespace Tabs {
class TabController;
}

#include <Widget.h>
#include "Tab.h"

namespace Tabs {

class TabController: public Widget {
public:
	TabController();
	~TabController();
	using Widget::Widget;

private:
	static class TabPool tabPool_;

	bool isMouseCaptured_;


	void transferCapture(std::shared_ptr<Tab>  movingTab);
};

} /* namespace Tabs */
#endif /* TABCONTROLLER_H_ */
