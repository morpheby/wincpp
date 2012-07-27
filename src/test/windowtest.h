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

#include "Window.h"
#include <cfloat>

#include <string>

#include <sstream>
using namespace std;

class MainWnd: public Window {
public:
	MainWnd(void);
	~MainWnd(void);
protected:
	bool WMClose();
	void WMSize();
private:
//	ButtonWnd genRnd, compPrivate;
//	EditboxWnd modulus, rKey, uKey;
//	LabelWnd status, lblMod, lblRKey, lblUKey;
//
//	EditboxWnd input, output;
//	ButtonWnd sign, check;

	int onGenRnd(Window& sender);
	int onCompPrivate(Window& sender);

	int onSign(Window& sender);
	int onCheck(Window& sender);

	int onModChange(Window& sender, const wstring& text);
	int onRKeyChange(Window& sender, const wstring& text);
	int onInputChange(Window& sender, const wstring& text);

	void Sizer();
};

#endif /* MAINWND_H_ */
