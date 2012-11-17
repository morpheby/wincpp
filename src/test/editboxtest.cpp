
#include <config.h>

#include <Widget.h>
#include <Window.h>
#include <EditboxWnd.h>

class MainWidget : public Widget {
public:
	MainWidget();
protected:
private:
	std::unique_ptr<EditboxWnd> input, output;

	int onInputChange(Window& sender, const std::wstring& text);
	int onWidgetReload(Widget &sender);
	int onGeometryChange(Widget &sender, WidgetEventParams &params);
	void checkNLoadComponents();
	void Sizer();
};

class Main {
public:
	Main() :
			widget{new MainWidget()} {
		widget->Show();
		widget->setEventHandler(WidgetEventType::close, NewEventExt(*this, &Main::OnWidgetClose));
	}

private:
	std::shared_ptr<Widget> widget;
	int OnWidgetClose(Widget &sender, WidgetEventParams &params) {
		PostQuitMessage(0);
		widget = nullptr;
		return 1;
	}
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<Main> mainClass (new Main());

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

void MainWidget::Sizer() {
	input->setPosition(10_scaled, 10_scaled);
	input->setSize(getWidth() - 15_scaled, 25_scaled);

	output->setPosition(10_scaled, input->getPosition().y+input->getSize().cy + 5_scaled);
	output->setSize(getWidth() - 15_scaled, 25_scaled);
}

int MainWidget::onInputChange(Window& sender, const std::wstring& text) {
	output->setText(text);
	return 0;
}

int MainWidget::onWidgetReload(Widget& sender) {
	checkNLoadComponents();
}

MainWidget::MainWidget()  :
		Widget(L"Test", 100_scaled, 100_scaled, 500_scaled, 500_scaled, getWindowDefaultStyle()) {
	setOnWidgetReload(NewEvent(*this, &MainWidget::onWidgetReload));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &MainWidget::onGeometryChange));
}

void MainWidget::checkNLoadComponents() {
	std::wstring oldText;
	if(input)
		oldText = input->getText();
	input = std::unique_ptr<EditboxWnd>(new EditboxWnd(L"Text field 1", 0, 0, 0, 0, getWindow()));
	output = std::unique_ptr<EditboxWnd>(new EditboxWnd(L"Text field 2...", 0, 0, 0, 0, getWindow()));

	output->setReadonly(true);

	Sizer();
	if(!oldText.empty()) {
		input->setText(oldText);
		output->setText(oldText);
	}
	input->Show();
	output->Show();

	input->setOnTextChange(NewEventExt(*this, &MainWidget::onInputChange));

}

int MainWidget::onGeometryChange(Widget& sender,
		WidgetEventParams& params) {
	Sizer();
}
