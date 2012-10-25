/*
 * ThemedDrawer.cpp
 *
 *  Created on: 24.09.2012
 *      Author: Morphe
 */

#include "ThemedDrawer.h"
#include "Window.h"

namespace Drawing {

ThemedDrawer::ThemedDrawer(const DC::DeviceContext dc, HTHEME theme) :
		Drawer(dc), theme_{theme}, bkgThemed_{false},
		bkgPartId_{0}, bkgStateId_{0} {
}

ThemedDrawer::~ThemedDrawer() {
}

void ThemedDrawer::setBackgroundThemed(int partId, int stateId) {
	bkgPartId_ = partId;
	bkgStateId_ = stateId;

}

void ThemedDrawer::setFontThemed(int partId, int stateId) {
	setFont(getThemeFont(partId, stateId));
}

HFONT ThemedDrawer::getThemeFont(int partId, int stateId) {
	LOGFONT font;

	if(::GetThemeFont(theme_, 0, partId, stateId, TMT_FONT, &font))
		return getDefFont();
	else
		return ::CreateFontIndirect(&font);
}

int ThemedDrawer::drawBackgroundInt(const RECT& rect,
		const RECT* clipRect) {
	if(!theme_)
		return Drawer::drawBackgroundInt(rect, clipRect);

	if (getDC().getOwner() && IsThemeBackgroundPartiallyTransparent(theme_, bkgPartId_, bkgStateId_))
		DrawThemeParentBackground(getDC().getOwner(), getDC(), 0);

	return DrawThemeBackground(theme_, getDC(), bkgPartId_, bkgStateId_, &rect, clipRect);
}

} /* namespace Drawing */
