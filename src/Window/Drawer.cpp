/*
 * Drawing.cpp
 *
 *  Created on: 23.09.2012
 *      Author: Morphe
 */

#include "Drawer.h"
#include <string>
#include "platform.h"

namespace Drawing {

Drawer::Drawer(const DC::DeviceContext& dc) :
	dc_{dc}, bkgnd_{getDefaultBkgndBrush()} {
}

Drawer::~Drawer() {
}

int Drawer::drawText(const std::wstring& text, DWORD dwFlags,
		RECT& rect) {
	return drawText_int(text, dwFlags, rect);
}

int Drawer::drawBackground(const RECT &rect) {
	return drawBackground_int(rect, 0);
}

int Drawer::drawBackground(const RECT &rect, const RECT &clipRect) {
	return drawBackground_int(rect, &clipRect);
}

int Drawer::fillRect(const RECT& rect, HBRUSH brush) {
	return ::FillRect(dc_, &rect, brush);
}

void Drawer::setBackgroundDefault() {
	setBackroundSolid(getDefaultBkgndBrush());
}

void Drawer::setBackroundSolid(HBRUSH brush) {
	DeleteObject(bkgnd_);
	bkgnd_ = brush;
}

void Drawer::setFontDefault() {
	setFont(getDefaultFont());
}

void Drawer::setFont(HFONT &&font) {
	DeleteObject(dc_.selectObject(font));
}

HFONT Drawer::getDefaultGuiFont() {
	return (HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
}

COLORREF Drawer::setTextColor(COLORREF color) {
	return SetTextColor(dc_, color);
}

HBRUSH Drawer::getDefWhiteBackgroundBrush() {
	return (HBRUSH) ::GetStockObject(WHITE_BRUSH);
}


int Drawer::drawBackground_int(const RECT& rect, const RECT* clipRect) {
	return fillRect(rect, bkgnd_);
}

HFONT Drawer::getDefaultFont() {
	return getDefFont_int();
}

HBRUSH Drawer::getDefaultBkgndBrush() {
	return getDefBkgndBrush_int();
}

int Drawer::drawText_int(const std::wstring& text, DWORD dwFlags,
		RECT& rect) {
	return ::DrawTextW(dc_, text.c_str(), -1, &rect, dwFlags);
}

HFONT Drawer::getDefFont_int() {
	return getDefaultGuiFont();
}

HBRUSH Drawer::getDefBkgndBrush_int() {
	return getDefWhiteBackgroundBrush();
}

COLORREF Drawer::getTextColor() {
	return GetTextColor(getDC());
}

void Drawer::clearFont() {
	setFont((HFONT) GetStockObject(SYSTEM_FONT));
}

LOGFONT Drawer::getLogFont(HFONT font) {
	LOGFONT logFont;
	GetObject(font, sizeof(LOGFONT), &logFont);
	return logFont;
}

}
