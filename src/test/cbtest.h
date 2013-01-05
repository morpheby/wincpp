/*
 * MainWnd.h
 *
 *  Created on: 22.3.2012
 *      Author: Morphe
 */

#ifndef MAINWND_H_
#define MAINWND_H_

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Window.h>
#include <cfloat>

#include <string>

#include <sstream>

#include <LabelWnd.h>
#include <ComboBox.h>
#include <EditboxWnd.h>
#include <sharedPtr.h>

using namespace std;
using namespace ComboBox;

wstring str_toUpper(wstring source);

struct _Uppercaser {
	wstring operator() (const shared_ptr<wstring> &pstr) const {
		return str_toUpper(*pstr);
	}
};

class MainWnd: public Window {
public:
	MainWnd(void);
	~MainWnd(void);
protected:
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
	bool WMClose();
	void WMSize();
private:
	typedef Selector<SimpleCB, shared_ptr<wstring>, _Uppercaser,
				_PtrLess<shared_ptr<wstring>>,
				_PtrEq<shared_ptr<wstring>> > _StrSelector;
	LabelWnd label;
	NamedSelector<DropDownListCB, shared_ptr<wstring>> cb1;
	NamedSelector<DropDownCB, shared_ptr<wstring>> cb2;
	_StrSelector cb3;

	EditboxWnd edit1, edit2;

	int onCBChange(Window &sender, shared_ptr<wstring> selection);
	int onSimpleCBChange(Window &sender, shared_ptr<wstring> selection);
	int onTextChange(Window &sender, const wstring &text);
	int onTextChange2(Window &sender, const wstring &text);
	shared_ptr<wstring> selection1, selection2;

	void Sizer();
};

#endif /* MAINWND_H_ */
