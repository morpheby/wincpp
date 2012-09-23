/*
 * widgettest.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Morphe
 */

#include <config.h>

#include <Widget.h>
#include <DeviceContext.h>

using DC::DeviceContext;

class MainWidget : public Widget {
public:
	MainWidget() :
		Widget(L"Test", getWindowDefaultStyle()) {

	}
protected:
	void DrawWindow(DC::DeviceContext dc) {
		dc.fillRect(RECT{0, 0, 100, 100}, (HBRUSH) GRAY_BRUSH);
	}
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<Widget> widget ( new Widget(L"Test",
			getWindowDefaultStyle()) );
	widget->Show();

	std::shared_ptr<MainWidget> widget2 ( new MainWidget() );
	widget2->Show();

	if(MessageBoxW(GetActiveWindow(), L"Are visual styles OK?",
			L"Test", MB_YESNO) == IDNO ||
		MessageBoxW(GetActiveWindow(), L"Do you see a window under message box?",
			L"Test", MB_YESNO) == IDNO)
		return 1;
	else
		MessageBoxW(GetActiveWindow(), L"Close that window",
					L"Test", MB_OK);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}




