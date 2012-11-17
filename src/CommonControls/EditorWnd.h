/*
 * EditorWnd.h
 *
 *  Created on: 23.5.2012
 *      Author: Morphe
 */

#ifndef EDITORWND_H_
#define EDITORWND_H_

#include "EditboxWnd.h"

class EditorWnd: public EditboxWnd {
public:
	EditorWnd(int x, int y, int width, int height, Window& parent);
	~EditorWnd();
};

#endif /* EDITORWND_H_ */
