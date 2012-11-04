/*
 * URLWnd.h
 *
 *  Created on: 31.05.2012
 *      Author: Morphe
 */

#ifndef URLWND_H_
#define URLWND_H_

#include "LabelWnd.h"
#include <NotifyWnd.h>

class URLWnd: public LabelWnd {
public:
	URLWnd();
	URLWnd(const std::wstring &url, int x, int y, HWND parentWnd);
	URLWnd(const std::wstring &url, const std::wstring &name,
			int x, int y, HWND parentWnd);
	~URLWnd();

	const std::wstring& getUrl() const;
	void setUrl(const std::wstring& url);
	bool isVisited() const;

protected:
	void PaintWindow(Drawing::Drawer &drawer) override;
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void CalcTxtRect() override;
private:
	short int state_;
	std::wstring url_;
	NotifyWnd urlFull_;
	bool mBtnDown_;

	void TrackMouseEvent(DWORD event);
};

#endif /* URLWND_H_ */
