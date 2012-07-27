/*
 * SimpleCB.cpp
 *
 *  Created on: 20.5.2012
 *      Author: Morphe
 */


#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "SimpleCB.h"

using namespace std;

namespace ComboBox {

SimpleCB::SimpleCB(int x, int y, int width, int height, Window& parent) :
		ComboBoxWnd(CBS_SIMPLE, x, y, width, height, parent), eb(getEditboxHandle()) {
	eb.setEmptyText(L"Поиск...");
	eb.setOnTextInput(NewEventExt(*this, &SimpleCB::EditboxAutocomplete));
}

SimpleCB::~SimpleCB() {
}

int SimpleCB::EditboxAutocomplete(Window& editbox, const wstring& text) {
	return Autocomplete(eb, text);
}

} /* namespace ComboBoxInternal */
