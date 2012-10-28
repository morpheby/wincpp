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
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	bool WMClose();
	void WMSize();
	void Sizer();
};

#endif /* MAINWND_H_ */
