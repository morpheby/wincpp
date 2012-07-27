/*
 * EditorWnd.cpp
 *
 *  Created on: 23.5.2012
 *      Author: Morphe
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "EditorWnd.h"

using namespace std;

EditorWnd::EditorWnd(int x, int y, int width, int height, Window& parent) :
	EditboxWnd(WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, L"", x, y, width, height, parent) {
}

EditorWnd::~EditorWnd() {
}

