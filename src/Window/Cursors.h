/*
 * Cursors.h
 *
 *  Created on: 03.08.2012
 *      Author: Morphe
 */

#ifndef CURSORS_H_
#define CURSORS_H_

#include <windows.h>

class Cursor {
public:
	Cursor(HCURSOR cur);

	void setCurrent();

	static void setCurrent(Cursor &cur);
	static void setNull();

	int getHeight() const;
	int getWidth() const;

	int getTop() const;
	int getBottom() const;
	int getLeft() const;
	int getRight() const;
private:
	HCURSOR cur_;
	int width, height;
	int hotPointX, hotPointY;
};

class CursorFactory {
public:
	static Cursor GetCursor(HINSTANCE app, LPTSTR cursorID);
	static Cursor GetCursor();
};

#endif /* CURSORS_H_ */
