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
	if(!isDropDownOpened()) {
		// BUGFIX: ComboBoxProc invokes its own autocomplete
		// on opening DropDown. Ask inhibition of WM_GETTEXT message
		eb.getEBInternal().InhibitMessage(WM_GETTEXT);
		openDropDown();
		eb.getEBInternal().RestoreMessage(WM_GETTEXT);
	}
	return Autocomplete(eb, text);
}

} /* namespace ComboBox */
