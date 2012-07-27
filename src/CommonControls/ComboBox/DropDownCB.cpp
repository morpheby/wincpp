/*
 * ComboBoxDD.cpp
 *
 *  Created on: 19.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "DropDownCB.h"

using namespace std;

namespace ComboBox {

DropDownCB::DropDownCB(int x, int y, int width, int height, Window& parent) :
				ComboBoxWnd(CBS_DROPDOWN, x, y, width, height, parent), eb(getEditboxHandle()) {
	eb.setEmptyText(L"Select an item...");
	eb.setOnTextInput(NewEventExt(*this, &DropDownCB::EditboxAutocomplete));
}

DropDownCB::~DropDownCB() {
}

void DropDownCB::openDropDownList() {
	openDropDown();
}

void DropDownCB::closeDropDownList() {
	closeDropDown();
}

int DropDownCB::EditboxAutocomplete(Window& editbox, const wstring& text) {
	openDropDown();
	return Autocomplete(eb, text);
}

} /* namespace ComboBox */
