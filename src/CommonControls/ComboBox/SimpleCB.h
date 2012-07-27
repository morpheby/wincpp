/*
 * SimpleCB.h
 *
 *  Created on: 20.5.2012
 *      Author: Morphe
 */

#ifndef SIMPLECB_H_
#define SIMPLECB_H_

#include "ComboBoxWnd.h"

namespace ComboBox {

class SimpleCB: public ComboBoxInternal::ComboBoxWnd {
public:
	SimpleCB(int x, int y, int width, int height, Window& parent);
	~SimpleCB();
private:
	int EditboxAutocomplete(Window& editbox, const std::wstring& text);
	EditboxWnd eb;
};

} /* namespace ComboBoxInternal */
#endif /* SIMPLECB_H_ */
