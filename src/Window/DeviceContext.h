/*
 * DeviceContext.h
 *
 *  Created on: 04.08.2012
 *      Author: Morphe
 */

#ifndef DEVICECONTEXT_H_
#define DEVICECONTEXT_H_

#include "DeviceContextBase.h"

/// Helper class for DC
class DeviceContext: public DeviceContextBase {
public:
	DeviceContext(const DeviceContextBase &dc);
	DeviceContext(DeviceContextBase &&dc);
	~DeviceContext();

	// Note that windows.h defines DrawText macro...
	virtual HRESULT DrawTextOut(int iPartId, int iStateId, const std::wstring& text,
			DWORD dwFlags, RECT& rect);
	HRESULT DrawTextOut(const std::wstring& text, DWORD dwFlags, RECT& rect);
	virtual HRESULT DrawBackground(int iPartId, int iStateId, const RECT& rect,
			const RECT* clipRect);
	HRESULT DrawBackground(int iPartId, int iStateId, const RECT& rect);
	virtual HRESULT FillRect(const RECT &rect, HBRUSH brush);

};

#endif /* DEVICECONTEXT_H_ */
