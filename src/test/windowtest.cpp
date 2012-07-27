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
				700_scaled, 500_scaled, 0, 0, 0)
//		genRnd(ButtonWnd::PushButton, L"Сгенерировать", 0, 0, 0, 0, *this),
//		compPrivate(ButtonWnd::PushButton, L"Вычислить r-key", 0, 0, 0, 0, *this),
//		status(L" ", 0, 0, *this),
//		lblMod(L"Модуль: ", 0, 0, *this),
//		lblRKey(L"Private-key: ", 0, 0, *this),
//		lblUKey(L"Public-key: ", 0, 0, *this),
//		input(L"Введите число...", 0, 0, 0, 0, *this),
//		output(L"Результат...", 0, 0, 0, 0, *this),
//		sign(ButtonWnd::DefPushButton, L"Зашифровать", 0, 0, 0, 0, *this),
//		check(ButtonWnd::PushButton, L"Дешифровать", 0, 0, 0, 0, *this) {
				{
	Sizer();
//	modulus.Show();
//	rKey.Show();
//	uKey.Show();
//
//	output.setReadonly(true);
//
//	input.Show();
//	output.Show();
//
//	genRnd.setOnClick(NewEvent(*this, &MainWnd::onGenRnd));
//	compPrivate.setOnClick(NewEvent(*this, &MainWnd::onCompPrivate));
//
//	sign.setOnClick(NewEvent(*this, &MainWnd::onSign));
//	check.setOnClick(NewEvent(*this, &MainWnd::onCheck));
//
//	modulus.setOnTextChange(NewEventExt(*this, &MainWnd::onModChange));
//	rKey.setOnTextChange(NewEventExt(*this, &MainWnd::onRKeyChange));
//	uKey.setOnTextChange(NewEventExt(*this, &MainWnd::onUKeyChange));
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
//	lblMod.SetPosition(10_scaled, 10_scaled);
//	lblRKey.SetPosition(10_scaled, 40_scaled);
//	lblUKey.SetPosition(10_scaled, 70_scaled);
//
//	set<int> widthSet = { lblMod.getWidth(), lblRKey.getWidth(), lblUKey.getWidth() };
//	int maxWidth = *widthSet.rbegin();
//
//	modulus.SetPosition(maxWidth + 12_scaled, 10_scaled);
//	modulus.setSize(getWidth() - rKey.getX() - 5_scaled, 25_scaled);
//
//	rKey.SetPosition(maxWidth + 12_scaled, 40_scaled);
//	rKey.setSize(getWidth() - rKey.getX() - 5_scaled, 25_scaled);
//	uKey.SetPosition(maxWidth + 12_scaled, 70_scaled);
//	uKey.setSize(getWidth() - rKey.getX() - 5_scaled, 25_scaled);
//
//	genRnd.SetPosition(10_scaled, 100_scaled);
//	genRnd.setSize(100_scaled, 25_scaled);
//
//	compPrivate.SetPosition(115_scaled, 100_scaled);
//	compPrivate.setSize(100_scaled, 25_scaled);
//
//	status.SetPosition(10_scaled, genRnd.getY()+genRnd.getHeight());
//
//	input.SetPosition(10_scaled, status.getY()+status.getHeight()+5_scaled);
//	input.setSize(getWidth() - 15_scaled, 25_scaled);
//
//	output.SetPosition(10_scaled, status.getY()+status.getHeight()+35_scaled);
//	output.setSize(getWidth() - 15_scaled, 25_scaled);
//
//	sign.SetPosition(10_scaled, output.getY()+output.getHeight()+5_scaled);
//	sign.setSize(90_scaled, 25_scaled);
//	check.SetPosition(105_scaled, output.getY()+output.getHeight()+5_scaled);
//	check.setSize(90_scaled, 25_scaled);
}

int MainWnd::onGenRnd(Window& sender) {
	return 0;
}

int MainWnd::onCompPrivate(Window& sender) {
	return 0;
}

int MainWnd::onSign(Window& sender) {
	return 0;
}

int MainWnd::onCheck(Window& sender) {
	return 0;
}

int MainWnd::onModChange(Window& sender, const wstring& text) {
	return 0;
}

int MainWnd::onRKeyChange(Window& sender, const wstring& text) {
	return 0;
}

int MainWnd::onInputChange(Window& sender, const wstring& text) {
	return 0;
}











