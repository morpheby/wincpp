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
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(const HDC& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(HDC&& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(const _DC_t& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(_DC_t&& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(const DeviceContextBase& dc) :
	DeviceContextBase(dc) {}
DeviceContext::DeviceContext(DeviceContextBase&& dc) :
	DeviceContextBase(dc) {}

DeviceContext::~DeviceContext() {
}

HRESULT DeviceContext::drawText(const std::wstring& text, DWORD dwFlags, RECT& rect) {
	return drawText(0, 0, text, dwFlags, rect);
}

HRESULT DeviceContext::drawText(int iPartId, int iStateId, const std::wstring& text,
		DWORD dwFlags, RECT& rect) {
	return ::DrawTextW(*this, text.c_str(), -1, &rect, dwFlags);
}

HRESULT DeviceContext::drawBackground(int iPartId, int iStateId,
		const RECT& rect, const RECT* clipRect) {
	return fillRect(rect, (HBRUSH) ::GetStockObject(WHITE_BRUSH));
}

HRESULT DeviceContext::fillRect(const RECT& rect, HBRUSH brush) {
	::FillRect(*this, &rect, brush);
}

HRESULT DeviceContext::drawBackground(int iPartId, int iStateId,
		const RECT& rect) {
	return drawBackground(iPartId, iStateId, rect, 0);
}

HFONT DeviceContext::getFont(int iPartId, int iStateId) {
	return (HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
}

HGDIOBJ DeviceContext::selectObject(HGDIOBJ obj) {
	return ::SelectObject(*this, obj);
}

}
