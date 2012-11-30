
#include "cbtest.h"
#include <cctype>


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
		cb1(5_scaled, 0, getSizeX()-10_scaled, 25_scaled, *this),
		cb2(5_scaled, 0, getSizeX()-10_scaled, 25_scaled, *this),
		cb3(5_scaled, 0, getSizeX()-10_scaled, 100_scaled, *this),
		edit1(L"Item text gets displayed here", 5_scaled, 0,
				getSizeX()-10_scaled, 25_scaled, *this),
		edit2(L"Item text from SimpleCB gets displayed here", 5_scaled, 0,
				getSizeX()-10_scaled, 25_scaled, *this) {

	label.setForcedWidth(true);
	cb1.setItems({
		{L"item1", SharePtr(new wstring(L"Hi from item1 :)"))},
		{L"item2", SharePtr(new wstring(L"Hi from item2 ;)"))},
		{L"item3", SharePtr(new wstring(L"Hi from item3 O:-)"))}
	});
	cb2.setItems({
		{L"first item", SharePtr(new wstring(L"Yay -- it's 1st ))"))},
		{L"first and a half item", SharePtr(new wstring(L"Heya )) Almost 1st :)"))},
		{L"Second item", SharePtr(new wstring(L"Just a bit off :("))},
		{L"Third item", SharePtr(new wstring(L"Looser :/"))}
	});
	cb3.setItems({
		SharePtr(new wstring(L"A These items are displayed")),
		SharePtr(new wstring(L"B upper-case in CB")),
		SharePtr(new wstring(L"B normally in the edit and"))
	});

	cb1.Show();
	cb2.Show();
	cb3.Show();
	edit1.Show();
	edit2.Show();

	label.setText(L"1. This line shall be automatically word-wrapped to the next "
			"line in case it doesn't fill the size of the window.\n"
			"2. Try resizing window -- word-wrapping shall occur right while you "
			"are changing size\n"
			"4. ComboBoxes shall move accordingly, so that text will not overlap the ComboBox\n"
			"5. Selecting an item in any CB shall change the text in Editbox\n"
			"6. Changes made in Editbox are stored\n"
			"7. Changing text in Editbox while an item from SimpleCB is selected changes item "
			"itself in ComboBox and its position\n"
			"8. DropDownCB and SimpleCB have auto-complete\n"
			"9. If everything happens right as described -- close the window and confirm test");

	cb1.setOnItemChange(NewEventExt(*this, &MainWnd::onCBChange));
	cb2.setOnItemChange(NewEventExt(*this, &MainWnd::onCBChange));
	cb3.setOnItemChange(NewEventExt(*this, &MainWnd::onSimpleCBChange));
	edit1.setOnTextChange(NewEventExt(*this, &MainWnd::onTextChange));
	edit2.setOnTextChange(NewEventExt(*this, &MainWnd::onTextChange2));
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
	label.setSizeX(getSizeX());
	label.ImmediatelyUpdateWindow();

	cb1.setPositionY(label.getCornerY()+5_scaled);
	cb1.setSizeX(getSizeX()-10_scaled);
	cb2.setPositionY(cb1.getCornerY()+5_scaled);
	cb2.setSizeX(getSizeX()-10_scaled);
	cb3.setPositionY(cb2.getCornerY()+5_scaled);
	cb3.setSizeX(getSizeX()-10_scaled);
	edit1.setPositionY(cb3.getCornerY()+5_scaled);
	edit1.setSizeX(getSizeX()-10_scaled);
	edit2.setPositionY(edit1.getCornerY()+5_scaled);
	edit2.setSizeX(getSizeX()-10_scaled);
}

int MainWnd::onCBChange(Window& sender, shared_ptr<wstring> pstr) {
	selection1 = pstr;
	if(selection1)
		edit1.setText(*selection1);
	else
		edit1.setText(L"");
}

int MainWnd::onSimpleCBChange(Window& sender, shared_ptr<wstring> pstr) {
	selection2 = pstr;
	if(selection2)
		edit2.setText(*selection2);
	else
		edit2.setText(L"");
}

wstring str_toUpper(wstring source) {
	for(auto i = source.begin(); i != source.end(); ++i)
		*i = towupper(*i);
	return source;
}

int MainWnd::onTextChange(Window& sender, const wstring& text) {
	if(selection1)
		*selection1 = text;
}

int MainWnd::onTextChange2(Window& sender, const wstring& text) {
	if(selection2)
		*selection2 = text;
	cb3.resetList();
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
