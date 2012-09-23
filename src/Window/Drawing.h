/*
 * Drawing.h
 *
 *  Created on: 23.09.2012
 *      Author: Morphe
 */

#ifndef DRAWING_H_
#define DRAWING_H_

#include "DeviceContext.h"

namespace Drawing {

class Drawer {
public:
	Drawer(const DC::DeviceContext &dc);
	Drawer(const DC::DeviceContext &dc, HTHEME hTheme);
	~Drawer();
private:
	DC::DeviceContext dc_;
	HTHEME hTheme_;
};

HRESULT drawText(int iPartId, int iStateId, const std::wstring& text,
		DWORD dwFlags, RECT& rect);

HRESULT drawText(const std::wstring& text, DWORD dwFlags, RECT& rect);
virtual HRESULT drawBackground(int iPartId, int iStateId, const RECT& rect,
		const RECT* clipRect);
HRESULT drawBackground(int iPartId, int iStateId, const RECT& rect);
virtual HRESULT fillRect(const RECT &rect, HBRUSH brush);

virtual HFONT getFont(int iPartId, int iStateId);

}


#endif /* DRAWING_H_ */
