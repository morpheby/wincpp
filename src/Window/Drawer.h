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
	virtual ~Drawer();
	int drawText(const std::wstring &text, DWORD dwFlags, RECT &rect);
	int drawBackground(const RECT &rect);
	int drawBackground(const RECT &rect, const RECT &clipRect);
	int fillRect(const RECT &rect, HBRUSH brush);
	void setBackgroundDefault();
	void setBackroundSolid(HBRUSH brush);
	void setFontDefault();
	void setFont(HFONT &&font);

	DC::DeviceContext getDC() {
		return dc_;
	}

	static HFONT getDefFont();
	static HBRUSH getDefBackgroundBrush();
protected:
	virtual int drawBackgroundInt(const RECT &rect, const RECT *clipRect);
	virtual int drawTextInt(const  std::wstring &text, DWORD dwFlags, RECT &rect);
private:
	DC::DeviceContext dc_;
	HBRUSH bkgnd_;
};

//HRESULT drawText(int iPartId, int iStateId, const std::wstring& text,
//		DWORD dwFlags, RECT& rect);



}


#endif /* DRAWING_H_ */
