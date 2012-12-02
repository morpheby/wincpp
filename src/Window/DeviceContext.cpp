/*
 * DeviceContext.cpp
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "DeviceContext.h"
#include <string>

namespace DC {

DeviceContext::DeviceContext(const DeviceContext& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(DeviceContext&& dc) :
	DeviceContextBase(std::move(dc)) {}
DeviceContext::DeviceContext(const HDC& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(HDC&& dc) :
	DeviceContextBase(std::move(dc)) {}
DeviceContext::DeviceContext(const _DC_t& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(_DC_t&& dc) :
	DeviceContextBase(std::move(dc)) {}
DeviceContext::DeviceContext(const DeviceContextBase& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(DeviceContextBase&& dc) :
	DeviceContextBase(std::move(dc)) {}

DeviceContext::~DeviceContext() {
}

HGDIOBJ DeviceContext::selectObject(HGDIOBJ obj) {
	return ::SelectObject(*this, obj);
}

}
