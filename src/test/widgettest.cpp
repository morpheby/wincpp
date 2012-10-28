/*
 * widgettest.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Morphe
 */

#include <config.h>

#include <Widget.h>
#include <DeviceContext.h>

class MainWidget : public Widget {
public:
	MainWidget() :
		Widget(L"Test", getWindowDefaultStyle()) {

	}
protected:
	void DrawWindow(Drawing::Drawer &drawer) {
		drawer.fillRect(RECT{0, 0, 100, 100}, (HBRUSH) GRAY_BRUSH);
	}
};

class Main {
public:
	Main() {
		widget1 = SharePtr(new Widget(L"Test", getWindowDefaultStyle()));
		widget1->Show();

		widget2 = SharePtr(new MainWidget());
		widget2->Show();
	}

private:
	std::shared_ptr<Widget> widget1, widget2;
	int OnWidget1Close(Widget &sender);
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<Main> mainClass (new Main());

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




