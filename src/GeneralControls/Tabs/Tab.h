/*
 * Tab.h
 *
 *  Created on: 30.11.2012
 *      Author: Morphe
 */

#ifndef TAB_H_
#define TAB_H_

#include <Widget.h>

namespace Tabs {

class Tab {
public:
	Tab();
	~Tab();
private:
	Widget *getThisWidget();
};

} /* namespace Tabs */
#endif /* TAB_H_ */
