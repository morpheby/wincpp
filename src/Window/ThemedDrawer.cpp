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

void ThemedDrawer::setFontThemed(int partId, int stateId, int propId) {
	setFont(getThemeFont(partId, stateId, propId));
}

HFONT ThemedDrawer::getThemeFont(int partId, int stateId, int propId) {
	LOGFONT font;

	if(getThemeLogFont_int(partId, stateId, propId, font)) // that most certainly won't
		return getDefaultGuiFont();				   // be good, if we recreate default fonts
	else
		return ::CreateFontIndirect(&font);
}

void ThemedDrawer::setBackgroundThemed(int partId, int stateId) {
	bkgPartId_ = partId;
	bkgStateId_ = stateId;
}

LOGFONT ThemedDrawer::getThemeLogFont(int partId, int stateId, int propId) {
	LOGFONT font;
	if(getThemeLogFont_int(partId, stateId, propId, font))
		return getLogFont(getDefaultGuiFont());
	else
		return font;

}

COLORREF ThemedDrawer::getThemeColor(int partId, int stateId, int propId) {
	COLORREF color;
	if(getThemeColor_int(partId, stateId, propId, color))
		return getTextColor();
	else
		return color;
}

HFONT ThemedDrawer::getDefFont_int() {
	return getThemeFont(TEXT_BODYTEXT, 0, TMT_FONT);
}

int ThemedDrawer::drawBackground_int(const RECT& rect,
		const RECT* clipRect) {
	if(!theme_) // all Parts have index starting from 1
		return Drawer::drawBackground_int(rect, clipRect);


	if(IsThemePartDefined(theme_, bkgPartId_, /*bkgStateId_*/0)) { // MSDN states stateId currently should be zero
		if (getDC().getOwner() && ::IsThemeBackgroundPartiallyTransparent(theme_, bkgPartId_, bkgStateId_))
			::DrawThemeParentBackground(getDC().getOwner(), getDC(), 0);
		return ::DrawThemeBackground(theme_, getDC(), bkgPartId_, bkgStateId_, &rect, clipRect);
	} else
		return Drawer::drawBackground_int(rect, clipRect);
}

BOOL ThemedDrawer::getThemeColor_int(int partId, int stateId, int propId, COLORREF &color) {
	return ::GetThemeColor(theme_, partId, stateId, propId, &color);
}

BOOL ThemedDrawer::getThemeLogFont_int(int partId, int stateId, int propId, LOGFONT& logFont) {
	return ::GetThemeFont(theme_, getDC(), partId, stateId, propId, &logFont);
}

} /* namespace Drawing */
