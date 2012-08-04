/*
 * DCFactory.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef DCFACTORY_H_
#define DCFACTORY_H_

#include "DeviceContextBase.h"

class DCFactory {
public:
	static DeviceContextBase GetDC(Window &wnd);
	static DeviceContextBase CreateCompatibleDC(const DeviceContextBase &dc);
};

#endif /* DCFACTORY_H_ */
