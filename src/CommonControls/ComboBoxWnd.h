/*
 * ComboBoxWnd.h
 *
 *  Created on: 19.5.2012
 *      Author: Morphe
 */

#ifndef COMBOBOXWND_H_
#define COMBOBOXWND_H_

#include <Window.h>
#include <EditboxWnd.h>
#include "CBWndInternal.h"

namespace ComboBoxInternal {

class ComboBoxWnd: public Window {
public:
	ComboBoxWnd();
	ComboBoxWnd(int style, int x, int y, int width, int height, Window& parent);
	~ComboBoxWnd();

	void setOnSelChange(WndEventExtBase<const std::wstring&> *onSelChange) {
		this->onSelChange_ = onSelChange;
	}

	void selString(const std::wstring& str);

protected:
	CBWndInternal& getCBInt() {
		return cb;
	}

	int findString(const std::wstring& str, int startPos = -1) const;
	std::wstring getItem(int i) const;

	void openDropDown();
	void closeDropDown();
	bool isDropDownOpened() const;
	HWND getEditboxHandle() const;

	template<class Container>
	void setStrings(const Container& strings);

	std::wstring getCurSel() const;
	int getCurSelNum() const;

	void setCurSelNum(int index);

	bool Autocomplete(EditboxWnd& eb, const std::wstring& inputText);

	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual int onSelChange(int index) {
		return onSelChange_(*this, getCurSel());
	}

private:
	CBWndInternal cb;
	WndEventExtCaller<const std::wstring&> onSelChange_;

	void WMSize(POINTS size);
};

template<class Container>
inline void ComboBoxWnd::setStrings(
		const Container& strings) {
	cb.setStrings(strings);
}


}

#endif /* COMBOBOXWND_H_ */
