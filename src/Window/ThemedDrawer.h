/*
 * ThemedDrawer.h
 *
 *  Created on: 24.09.2012
 *      Author: Morphe
 */

#ifndef THEMEDDRAWER_H_
#define THEMEDDRAWER_H_

#include "Drawer.h"

class Window;
#ifndef HANDLE
typedef void *HANDLE;
#endif

#ifndef HTHEME
typedef HANDLE HTHEME;
#endif

namespace Drawing {

class ThemedDrawer: public Drawing::Drawer {
public:
	ThemedDrawer(const DC::DeviceContext dc, HTHEME theme);
	~ThemedDrawer();

	void setBackgroundThemed(int partId, int stateId); // see MSDN for "Parts and States"
	void setFontThemed(int partId, int stateId, int propId);

	HFONT getThemeFont(int partId, int stateId, int propId);
	LOGFONT getThemeLogFont(int partId, int stateId, int propId);
	COLORREF getThemeColor(int partId, int stateId, int propId);

	int drawThemedEdges(RECT &rect, int partId, int stateId, UINT style, UINT flags);
protected:
	int drawBackground_int(const RECT &rect, const RECT *clipRect);
	HFONT getDefFont_int();

	BOOL getThemeColor_int(int partId, int stateId, int propId, COLORREF &color);
	BOOL getThemeLogFont_int(int partId, int stateId, int propId, LOGFONT &logFont);
private:
	HTHEME theme_;
	int bkgPartId_, bkgStateId_;
};

} /* namespace Drawing */
#endif /* THEMEDDRAWER_H_ */
