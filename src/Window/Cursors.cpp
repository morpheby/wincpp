/*
 * Cursors.cpp
 *
 *  Created on: 03.08.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#	include <config.h>
#endif

#include "Cursors.h"

Cursor::Cursor(HCURSOR cur) :
	cur_(cur) {
	ICONINFO icon;
	BITMAP iconBmp;
	GetIconInfo(cur, &icon);


	if(icon.hbmColor) {
		GetObject(icon.hbmColor, sizeof(BITMAP), &iconBmp);
		height = iconBmp.bmHeight;
		DeleteObject(icon.hbmColor);
	} else {
		GetObject(icon.hbmMask, sizeof(BITMAP), &iconBmp);
		height = iconBmp.bmHeight / 2;
	}
	width = iconBmp.bmWidth;
	DeleteObject(icon.hbmMask);

	hotPointX = icon.xHotspot;
	hotPointY = icon.yHotspot;
}

void Cursor::setCurrent() {
	setCurrent(*this);
}

void Cursor::setCurrent(Cursor& cur) {
	SetCursor(cur.cur_);
}

void Cursor::setNull() {
	SetCursor(0);
}

int Cursor::getHeight() const {
	return height;
}

int Cursor::getWidth() const {
	return width;
}

int Cursor::getTop() const {
	return - hotPointY;
}

int Cursor::getBottom() const {
	return height - hotPointY;
}

int Cursor::getLeft() const {
	return - hotPointX;
}

int Cursor::getRight() const {
	return width - hotPointX;
}

Cursor CursorFactory::GetCursor(HINSTANCE app, LPTSTR cursorID) {
	return LoadCursor(app, cursorID);
}

Cursor CursorFactory::GetCursor() {
	return ::GetCursor();
}
