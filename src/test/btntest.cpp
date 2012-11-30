
#include "btntest.h"


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

	return (int) msg.wParam;
}


MainWnd::MainWnd(void) :
		Window(L"Test", WS_OVERLAPPEDWINDOW, 50_scaled, 50_scaled,
				700_scaled, 500_scaled, 0, 0, 0),
		label(L"", 0, 0, *this),
		btnOK(ButtonWnd::DefPushButton, L"OK",
				5_scaled, 0, 60_scaled, 25_scaled, *this),
		btnCancel(ButtonWnd::PushButton, L"Cancel",
				btnOK.getRight()+5_scaled, 0, 60_scaled, 25_scaled, *this)
		{

	label.setForcedWidth(true);

	label.setText(L"1. This line shall be automatically word-wrapped to the next "
			"line in case it doesn't fill the size of the window.\n"
			"2. Try resizing window -- word-wrapping shall occur right while you "
			"are changing size\n"
			"3. There two adjacent buttons - \'OK\' and \'Cancel\'\n"
			"4. As this label resizes, they move accordingly, thus never "
			"overlapping each other\n"
			"5. If everything happens right as described -- press OK button, "
			"else press \'Cancel\' button or close the window");

	btnOK.setOnClick(NewEvent(*this, &MainWnd::onOKClick));
	btnCancel.setOnClick(NewEvent(*this, &MainWnd::onCancelClick));
	Sizer();
}

MainWnd::~MainWnd(void) {
}

bool MainWnd::WMClose() {
	PostQuitMessage(1); // This is the wrong way to close that window
	return true;
}

void MainWnd::WMSize() {
	Sizer();
}

void MainWnd::Sizer() {
	label.setWidth(getWidth());
	label.ImmediatelyUpdateWindow();

	btnOK.setY(label.getBottom()+5_scaled);
	btnCancel.setY(label.getBottom()+5_scaled);
}

int MainWnd::onOKClick(Window& sender) {
	PostQuitMessage(0);
	return 0;
}

int MainWnd::onCancelClick(Window& sender) {
	PostQuitMessage(1);
	return 0;
}
