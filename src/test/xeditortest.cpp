
#include "xeditortest.h"


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
		editor_(0, 0, getSizeX(), getSizeY(), *this) {

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
	editor_.setSize(getSize());
	editor_.ImmediatelyUpdateWindow();
}

LRESULT MainWnd::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_SIZE:
		WMSize();
		break;
	case WM_CLOSE:
		WMClose();
		break;
	case WM_SETFOCUS:
		SetFocus(editor_);
		return 0;
	}
	return Window::WndProc(msg, wParam, lParam);
}
