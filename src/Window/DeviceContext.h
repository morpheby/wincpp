/*
 * DeviceContext.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef DEVICECONTEXT_H_
#define DEVICECONTEXT_H_

#include "DeviceContextBase.h"

namespace DC {

/// Helper class for DC
class DeviceContext: public DeviceContextBase {
public:
	DeviceContext(const DeviceContext &dc);
	DeviceContext(DeviceContext &&dc);
	~DeviceContext();

	DeviceContext(const HDC &dc);
	DeviceContext(HDC &&dc); // this ctor gains exclusive access
	DeviceContext(const _DC_t &dc);
	DeviceContext(_DC_t &&dc);
	DeviceContext(const DeviceContextBase &dc);
	DeviceContext(DeviceContextBase &&dc);

	HGDIOBJ selectObject(HGDIOBJ obj);
};

}

#endif /* DEVICECONTEXT_H_ */
