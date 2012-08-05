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

	// Note that windows.h defines DrawText macro...
	virtual HRESULT drawText(int iPartId, int iStateId, const std::wstring& text,
			DWORD dwFlags, RECT& rect);
	HRESULT drawText(const std::wstring& text, DWORD dwFlags, RECT& rect);
	virtual HRESULT drawBackground(int iPartId, int iStateId, const RECT& rect,
			const RECT* clipRect);
	HRESULT drawBackground(int iPartId, int iStateId, const RECT& rect);
	virtual HRESULT fillRect(const RECT &rect, HBRUSH brush);

	virtual HFONT getFont(int iPartId, int iStateId);

	HGDIOBJ selectObject(HGDIOBJ obj);
};

}

#endif /* DEVICECONTEXT_H_ */
