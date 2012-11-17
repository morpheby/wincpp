/*
 * widgettest.cpp
 *
 *  Created on: 11.08.2012
 *      Author: Morphe
 */

#include <config.h>

#include <Widget.h>
#include <Window.h>

class MainWidget : public Widget {
public:
	MainWidget() :
		Widget(L"Gray square", 100_scaled, 100_scaled, 500_scaled, 500_scaled, getWindowDefaultStyle()) {

	}
protected:
	void DrawWindow(Drawing::Drawer &drawer) {
		drawer.fillRect(RECT{0, 0, 100, 100}, (HBRUSH) GetStockObject(GRAY_BRUSH));
	}
};

class Main {
public:
	Main() :
			widget1{new Widget(L"Blank window", 50_scaled, 50_scaled,
					500_scaled, 500_scaled, getWindowDefaultStyle())} {
		widget1->Show();

		widget2 = SharePtr(new MainWidget());
		widget2->Show();
		widget1->setEventHandler(WidgetEventType::close, NewEventExt(*this, &Main::OnWidget1Close));
	}

private:
	std::shared_ptr<Widget> widget1, widget2;
	int OnWidget1Close(Widget &sender, WidgetEventParams &params) {
		PostQuitMessage(0);
		widget1 = nullptr;
		return 1;
	}
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<Main> mainClass (new Main());

	MessageBoxW(GetActiveWindow(),
			L"1. There are two windows underneath,\n"
			 "   one is \'Blank window\' and another is \'Gray square\'\n"
			 "2. Nothing is displayed in the first window\n"
			 "3. The second window has small gray square in the upper-left corner\n"
			 "4. Closing \'Gray square\' window makes it to reload itself\n"
			 "5. After minimizing, closing (repeatedly), and then restoring \'GS\' window\n"
			 "   you will get that window at the position before minimizing it\n"
			 "6. Same with maximizing\n"
			 "7. Close \'Blank window\' to end test",
			L"Instructions", MB_OK);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(MessageBoxW(GetActiveWindow(), L"Was everything according to the instructions?",
			L"Test", MB_YESNO) == IDNO)
		return 1;

	return (int) msg.wParam;
}




