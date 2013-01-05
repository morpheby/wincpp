/*
 * DCFactory.cpp
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#include "DCFactory.h"

#include "Window.h"

namespace DC {

DeviceContextBase GetDC(Window& wnd) {
	return _DC_t{::GetDC(wnd), wnd};
}

DeviceContextBase CreateCompatibleDC(const DeviceContextBase& dc) {
	return ::CreateCompatibleDC(dc);
}

}
