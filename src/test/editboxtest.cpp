
#include "editboxtest.h"


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
			L"1. The window underneath should have two editboxes,\n"
			 "   one has \'Text field 1\' and another has \'Text field 2...\'\n"
			 "2. TF2 is read-only\n"
			 "3. Setting focus to TF1 erases \'Text field 1\'\n"
			 "4. Taking focus from TF1 while it is emty sets \'Text field 1\' again\n"
			 "5. Entering text in TF1 sets exactly same text in TF2\n"
			 "6. While TF1 displays \'Text field 1\' TF2 will ALWAYS \n"
			 "   display \'Text field 2...\'",
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
		input(L"Text field 1", 0, 0, 0, 0, *this),
		output(L"Text field 2...", 0, 0, 0, 0, *this) {
	Sizer();
	output.setReadonly(true);

	input.Show();
	output.Show();
	input.setOnTextChange(NewEventExt(*this, &MainWnd::onInputChange));
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
	input.SetPosition(10_scaled, 10_scaled);
	input.setSize(getWidth() - 15_scaled, 25_scaled);

	output.SetPosition(10_scaled, input.getY()+input.getHeight()+5_scaled);
	output.setSize(getWidth() - 15_scaled, 25_scaled);
}

int MainWnd::onInputChange(Window& sender, const wstring& text) {
	output.setText(text);
	return 0;
}











