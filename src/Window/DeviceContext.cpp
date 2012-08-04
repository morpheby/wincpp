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

DeviceContext::DeviceContext(const DeviceContextBase& dc) :
	DeviceContextBase(dc) {
}

DeviceContext::DeviceContext(DeviceContextBase&& dc) :
	DeviceContextBase(dc) {
}

DeviceContext::~DeviceContext() {
}

HRESULT DeviceContext::DrawTextOut(const std::wstring& text, DWORD dwFlags, RECT& rect) {
	return DrawTextOut(0, 0, text, dwFlags, rect);
}

HRESULT DeviceContext::DrawTextOut(int iPartId, int iStateId, const std::wstring& text,
		DWORD dwFlags, RECT& rect) {
	return ::DrawTextW(*this, text.c_str(), -1, &rect, dwFlags);
}

HRESULT DeviceContext::DrawBackground(int iPartId, int iStateId,
		const RECT& rect, const RECT* clipRect) {
	return FillRect(rect, (HBRUSH) GetStockObject(WHITE_BRUSH));
}

HRESULT DeviceContext::FillRect(const RECT& rect, HBRUSH brush) {
	::FillRect(*this, &rect, brush);
}

HRESULT DeviceContext::DrawBackground(int iPartId, int iStateId,
		const RECT& rect) {
	return DrawBackground(iPartId, iStateId, rect, 0);
}
