/*
 * ComboBoxDD.h
 *
 *  Created on: 19.5.2012
 *      Author: Morphe
 */

#ifndef COMBOBOXDD_H_
#define COMBOBOXDD_H_

#include "ComboBoxWnd.h"

namespace ComboBox {

class DropDownCB : public ComboBoxInternal::ComboBoxWnd {
public:
	DropDownCB(int x, int y, int width, int height, Window& parent);
	~DropDownCB();

	void openDropDownList();
	void closeDropDownList();
private:
	int EditboxAutocomplete(Window& editbox, const std::wstring& text);
	EditboxWnd eb;
};

} /* namespace ComboBox */
#endif /* COMBOBOXDD_H_ */
