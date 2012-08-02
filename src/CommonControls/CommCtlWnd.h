#ifndef COMMCTLWND_H_
#define COMMCTLWND_H_

#include <Window.h>
#include <errorParse.h>
#include <set>


class CommCtlWnd: public Window {
public:
	CommCtlWnd(std::wstring commCtlClass);
	CommCtlWnd(std::wstring name, UINT style, HWND wndParent, int x, int y,
			int width, int height, std::wstring commCtlClass);
	CommCtlWnd(HWND convertFrom);
	~CommCtlWnd(void);

	void InhibitMessage(UINT msg);
	void RestoreMessage(UINT msg);

protected:
	std::wstring GetThemeApplicableClassList();
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT DefWndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	HWND CreateWnd(const std::wstring& wndName, UINT style, HWND parentWnd, HMENU menu, HINSTANCE instance,
			LPVOID lpParam);
	void WMKeyDown(int vk);
private:
	WNDPROC commCtlProc;
	std::wstring commCtlClass;
	HWND converted;
	std::set<UINT> inhibitedMsg;
	void setDefFont();
};

#endif /* COMMCTLWND_H_ */
