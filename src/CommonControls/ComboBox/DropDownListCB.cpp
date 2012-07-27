/*
 * DropDownListCB.cpp
 *
 *  Created on: 22.05.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "DropDownListCB.h"

using namespace std;

namespace ComboBox {

DropDownListCB::DropDownListCB(int x, int y, int width, int height,
		Window& parent) :
		ComboBoxWnd(CBS_DROPDOWNLIST, x, y, width, height, parent) {
}

DropDownListCB::~DropDownListCB() {
}

} /* namespace ComboBox */
