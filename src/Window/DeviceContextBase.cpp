/*
 * DeviceContext.cpp
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "DeviceContextBase.h"

using std::shared_ptr;

namespace DC {

struct _dc_deleter {
	void operator() (_DC_t *pdc) {
		if(pdc->owner)
			ReleaseDC(pdc->owner, pdc->dc);
		else
			DeleteDC(pdc->dc);

		delete pdc;
        }
};

DeviceContextBase::DeviceContextBase(const HDC & dc)
		: pdc_{std::make_shared<_DC_t>(_DC_t{dc, 0})} {
}

DeviceContextBase::DeviceContextBase(HDC && dc)
		: pdc_{new _DC_t{dc, 0}, _dc_deleter()} {
}

DeviceContextBase::DeviceContextBase(const DeviceContextBase & dc)
		: pdc_{dc.pdc_} {
}

DeviceContextBase::DeviceContextBase(DeviceContextBase && dc)
		: pdc_{std::move(dc.pdc_)} {
}

DeviceContextBase::DeviceContextBase(const _DC_t & dc)
		:pdc_{std::make_shared<_DC_t>(dc)} {
}

DeviceContextBase::DeviceContextBase(_DC_t && dc)
		:pdc_{new _DC_t(dc), _dc_deleter()} {
}

DeviceContextBase::~DeviceContextBase() {
}

DeviceContextBase::operator HDC() const {
	return pdc_->dc;
}

HWND DeviceContextBase::getOwner() const {
	return pdc_->owner;
}

}
