/*
 * RenameWnd.h
 *
 *  Created on: 01.06.2012
 *      Author: Morphe
 */

#ifndef RENAMEWND_H_
#define RENAMEWND_H_

#include <Window.h>
#include <EditboxWnd.h>
#include <ButtonWnd.h>

class RenameWnd: public Window {
public:
	RenameWnd(const std::wstring& str, int x, int y, int width, int height, Window& parent);
	~RenameWnd();

	std::wstring getStr() const {
		return str_;
	}

private:
	std::wstring str_;
	EditboxWnd edit;
	ButtonWnd okBtn;
	bool WMClose();
	void WMDestroy();
	int OkClicked(Window& wnd);
};

#endif /* RENAMEWND_H_ */
