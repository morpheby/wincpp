/*
 * CBWndInternal.h
 *
 *  Created on: 20.5.2012
 *      Author: Morphe
 */

#ifndef CBWNDINTERNAL_H_
#define CBWNDINTERNAL_H_

#include <CommCtlWnd.h>
#include <EditboxWnd.h>

namespace ComboBoxInternal {

class CBWndInternal : public CommCtlWnd  {
public:
	CBWndInternal();
	CBWndInternal(int style, int x, int y, int width, int height, Window &parent);
	~CBWndInternal();

	template<class Container>
	void setStrings(const Container& strings);

	std::wstring getCurSel() const;
	int getCurSelNum() const;
	void setCurSelNum(int index);
	std::wstring getItem(int i) const;

	int findString(const std::wstring& str, int startPos = -1) const;

	void selString(const std::wstring& str);

	void openDropDown();
	void closeDropDown();
	bool getDropDownState() const;

private:
	void addString(const std::wstring& str);
	void clearList();
	int getStrSize(int index) const;
};

template<class Container>
inline void CBWndInternal::setStrings(const Container& strings) {
	setRedraw(false);
	clearList();
	for(auto& s : strings)
		addString((std::wstring)s);
	setRedraw(true);
}

} /* namespace ComboBoxInternal */
#endif /* CBWNDINTERNAL_H_ */
