/*
 * DeviceContext.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef DEVICECONTEXT_H_
#define DEVICECONTEXT_H_

#include <Window.h>
#include <memory>
#include <sharedPtr.h>

struct _DC_t {
	HDC dc;
	HWND owner;
};

/// Provides safe acquire, copy and release mechanics for HDC
class DeviceContextBase {
public:
	DeviceContextBase(const HDC &dc);
	DeviceContextBase(HDC &&dc); // this ctor gains exclusive access
	DeviceContextBase(const _DC_t &dc);
	DeviceContextBase(_DC_t &&dc);
	DeviceContextBase(const DeviceContextBase &dc);
	DeviceContextBase(DeviceContextBase &&dc);
	virtual ~DeviceContextBase();

	operator HDC() const;
private:
	std::shared_ptr<_DC_t> pdc_;
};

#endif /* DEVICECONTEXT_H_ */
