#ifndef NOTIFY_WND_H_
#define NOTIFY_WND_H_

#include <window.h>

class NotifyWnd: public Window {
public:
	NotifyWnd(void);
	NotifyWnd(const std::wstring &text, int x, int y, HWND parentWnd);
	~NotifyWnd(void);
	void setText(const std::wstring &text);
protected:
	void PaintWindow(Drawing::Drawer &drawer) override;
	bool WMEraseBackground(Drawing::Drawer &drawer);
	std::wstring GetThemeApplicableClassList();
private:
	std::wstring txt;
	RECT txtRect;
};

#endif /* NOTIFY_WND_H_ */
