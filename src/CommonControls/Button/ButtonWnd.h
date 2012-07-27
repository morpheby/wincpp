#ifndef BUTTON_WND_H_
#define BUTTON_WND_H_

#include <windows.h>
#include <CommCtrl.h>

#include <CommCtlWnd.h>
#include <Events.h>



class ButtonWnd: public CommCtlWnd {
public:
	enum ButtonType {
		PushButton = BS_PUSHBUTTON,
		DefPushButton = BS_DEFPUSHBUTTON,
		CheckBox = BS_CHECKBOX,
		RadioButton = BS_RADIOBUTTON
	};
	ButtonWnd(void);
	ButtonWnd(ButtonType btnType);
	ButtonWnd(ButtonType btnType, std::wstring name, int x, int y, int width,
			int height, HWND parent);
	~ButtonWnd(void);
	void SetButtonType(ButtonType btnType);
	void setOnClick(WndEventBase *onClick);
private:
	WndEventCaller onClick;
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif /* BUTTON_WND_H_ */
