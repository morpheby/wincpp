#include "windowtest.h"
//#include "LabelWnd.h"
//#include "EditboxWnd.h"
//#include "ComboBox.h"
//#include "EditorWnd.h"


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


MainWnd::MainWnd(void) :
		Window(L"Test", WS_OVERLAPPEDWINDOW, 50_scaled, 50_scaled,
				700_scaled, 500_scaled, 0, 0, 0) {
	Sizer();
}

MainWnd::~MainWnd(void) {
}

bool MainWnd::WMClose() {
	// Since we've managed to this part of code,
	// test succeeded
	PostQuitMessage(0);
	return true;
}

void MainWnd::WMSize() {
	Sizer();
}

void MainWnd::Sizer() {
}

LRESULT MainWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CLOSE:
		WMClose();
		break;
	case WM_SIZE:
		WMSize();
		break;
	}
	return Window::WndProc(msg, wParam, lParam);
}
