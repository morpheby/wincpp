
#include "urltest.h"


int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MainWnd window;
	if (!window.getWindowHandle()) {
		// If window was not initialized, test failed
		return 1;
	}
	window.Show(nCmdShow);

	MessageBoxW(GetActiveWindow(),
			L"1. The window underneath should have label,\n"
			 "   which will provide further instructions",
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


MainWnd::MainWnd(void) :
		Window(L"Test", WS_OVERLAPPEDWINDOW, 50_scaled, 50_scaled,
				700_scaled, 500_scaled, 0, 0, 0),
		label(L"", 0, 0, *this),
		urlWeb(L"http://www.google.com", 2_scaled, 0, *this),
		urlNamedWeb(L"http://www.microsoft.com", L"Microsoft", 2_scaled, 0, *this),
		urlApp(L"notepad.exe", L"Open Notepad", 2_scaled, 0, *this) {

	label.setForcedWidth(true);

	label.setText(L"1. This line shall be automatically word-wrapped to the next "
			"line in case it doesn't fill the size of the window.\n"
			"2. Try resizing window -- word-wrapping shall occur right while you "
			"are changing size\n"
			"3. There are three links: the first points to web site, the second points to "
			"the same web site, but displays a name and the third opens notepad and "
			"is named \'Open Notepad\'\n"
			"4. Hovering the mouse over a link shows tooltip with full URL\n"
			"5. Pressing the link once darkens its colour\n"
			"6. If everything happens right as described -- close the window and confirm test");

	Sizer();
}

MainWnd::~MainWnd(void) {
}

bool MainWnd::WMClose() {
	PostQuitMessage(0);
	return true;
}

void MainWnd::WMSize() {
	Sizer();
}

void MainWnd::Sizer() {
	label.setWidth(getWidth());
	label.ImmediatelyUpdateWindow();

	urlWeb.setY(label.getBottom());
	urlNamedWeb.setY(urlWeb.getBottom());
	urlApp.setY(urlNamedWeb.getBottom());
}












