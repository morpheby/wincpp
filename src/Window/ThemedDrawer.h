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
	void setFontThemed(int partId, int stateId);

	HFONT getThemeFont(int partId, int stateId);
	static HFONT getThemeFont(HTHEME theme, int partId, int stateId);
protected:
	int drawBackgroundInt(const RECT &rect, const RECT *clipRect);
private:
	HTHEME theme_;
	bool bkgThemed_;
	int bkgPartId_, bkgStateId_;
};

} /* namespace Drawing */
#endif /* THEMEDDRAWER_H_ */
