/*
 * ThemedDrawer.cpp
 *
 *  Created on: 24.09.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "ThemedDrawer.h"
#include "Window.h"

namespace Drawing {

ThemedDrawer::ThemedDrawer(const DC::DeviceContext dc, HTHEME theme) :
		Drawer(dc), theme_{theme},
		bkgPartId_{0}, bkgStateId_{0} {
}

ThemedDrawer::~ThemedDrawer() {
}

void ThemedDrawer::setFontThemed(int partId, int stateId) {
	setFont(getThemeFont(partId, stateId));
}

HFONT ThemedDrawer::getThemeFont(int partId, int stateId) {
	LOGFONT font;

	if(getThemeLogFont(partId, stateId, font))
		return getDefaultGuiFont();
	else
		return ::CreateFontIndirect(&font);
}

void ThemedDrawer::setBackgroundThemed(int partId, int stateId) {
	bkgPartId_ = partId;
	bkgStateId_ = stateId;
}

BOOL ThemedDrawer::getThemeLogFont(int partId, int stateId, LOGFONT& logFont) {
	::GetThemeFont(theme_, 0, partId, stateId, TMT_FONT, &logFont);
}

HFONT ThemedDrawer::getDefFont_int() {
	return getThemeFont(TEXT_BODYTEXT, 0);
}

int ThemedDrawer::drawBackground_int(const RECT& rect,
		const RECT* clipRect) {
	if(!bkgPartId_ || !theme_) // all Parts have index starting from 1
		return Drawer::drawBackground_int(rect, clipRect);

	if (getDC().getOwner() && IsThemeBackgroundPartiallyTransparent(theme_, bkgPartId_, bkgStateId_))
		DrawThemeParentBackground(getDC().getOwner(), getDC(), 0);

	return DrawThemeBackground(theme_, getDC(), bkgPartId_, bkgStateId_, &rect, clipRect);
}

} /* namespace Drawing */
