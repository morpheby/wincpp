/*
 * DropDownListCB.h
 *
 *  Created on: 22.05.2012
 *      Author: Morphe
 */

#ifndef DROPDOWNLISTCB_H_
#define DROPDOWNLISTCB_H_

#include "ComboBoxWnd.h"

namespace ComboBox {

class DropDownListCB : public ComboBoxInternal::ComboBoxWnd{
public:
	DropDownListCB(int x, int y, int width, int height, Window& parent);
	~DropDownListCB();
};

} /* namespace ComboBox */
#endif /* DROPDOWNLISTCB_H_ */
