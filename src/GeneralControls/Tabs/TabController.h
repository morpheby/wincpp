/*
 * TabController.h
 *
 *  Created on: 23.11.2012
 *      Author: Morphe
 */

#ifndef TABCONTROLLER_H_
#define TABCONTROLLER_H_

#include <Widget.h>
#include "Tab.h"

namespace Tabs {

class TabController: public Widget {
public:
	TabController();
	~TabController();


private:
	static class TabPool tabPool_;

	bool isMouseCaptured_;


	void TransferCapture(std::shared_ptr<Tab> movingTab);
};

} /* namespace Tabs */
#endif /* TABCONTROLLER_H_ */
