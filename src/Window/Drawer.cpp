/*
 * Drawing.cpp
 *
 *  Created on: 23.09.2012
 *      Author: Morphe
 */

#include "Drawer.h"
#include "platform.h"

namespace Drawing {

Drawer::Drawer(const DC::DeviceContext& dc) :
	dc_{dc}, bkgnd_{getDefBackgroundBrush()} {
}

Drawer::~Drawer() {
}

int Drawer::drawText(const std::wstring& text, DWORD dwFlags,
		RECT& rect) {
	return drawTextInt(text, dwFlags, rect);
}

int Drawer::drawBackground(const RECT &rect) {
	return drawBackgroundInt(rect, 0);
}

int Drawer::drawBackground(const RECT &rect, const RECT &clipRect) {
	return drawBackgroundInt(rect, &clipRect);
}

int Drawer::fillRect(const RECT& rect, HBRUSH brush) {
	return ::FillRect(dc_, &rect, brush);
}

void Drawer::setBackgroundDefault() {
	setBackroundSolid(getDefBackgroundBrush());
}

void Drawer::setBackroundSolid(HBRUSH brush) {
	DeleteObject(bkgnd_);
	bkgnd_ = brush;
}

void Drawer::setFontDefault() {
	setFont(getDefFont());
}

void Drawer::setFont(HFONT &&font) {
	DeleteObject(dc_.selectObject(font));
}

static HFONT Drawer::getDefFont() {
	return (HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
}

static HBRUSH Drawer::getDefBackgroundBrush() {
	return (HBRUSH) ::GetStockObject(WHITE_BRUSH);
}


int Drawer::drawBackgroundInt(const RECT& rect, const RECT* clipRect) {
	return fillRect(rect, bkgnd_);
}

int Drawer::drawTextInt(const std::wstring& text, DWORD dwFlags,
		RECT& rect) {
	return ::DrawTextW(dc_, text.c_str(), -1, &rect, dwFlags);
}

}
