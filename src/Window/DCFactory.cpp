/*
 * DCFactory.cpp
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#include "DCFactory.h"

namespace DC {

DeviceContextBase DCFactory::GetDC(Window& wnd) {
	return {::GetDC(wnd), wnd};
}

DeviceContextBase DCFactory::CreateCompatibleDC(const DeviceContextBase& dc) {
	return ::CreateCompatibleDC(dc);
}

}
