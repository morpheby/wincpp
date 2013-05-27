
#include <config.h>

#include <Widget.h>
#include <Window.h>
#include <EditboxWnd.h>
#include <TabController.h>
#include <TabWidget.h>
#include <fstream>

using Tabs::TabWidget;

class MainWidget : public TabWidget {
public:
	MainWidget(const std::wstring &name);
	MainWidget();
protected:
	void RegisterFields() override;
private:
	std::wstring text_;
	std::unique_ptr<EditboxWnd> input, output;

	int onInputChange(Window& sender, const std::wstring& text);
	int onWidgetReload(Widget &sender);
	int onGeometryChange(Widget &sender, WidgetEventParams &params);
	void checkNLoadComponents();
	void Sizer();

	int TextDeserialize(serializing::_internal::SFieldNotifying<std::wstring> &sender);
};

class Main {
public:
	Main() :
		tabController_{std::make_shared<Tabs::TabController>(100_scaled, 100_scaled, 500_scaled, 500_scaled, getWindowDefaultStyle())} {

		std::fstream state("serialtest.state", std::ios::in | std::ios::binary);
		if(state.is_open()) {
			tabController_->Deserialize(state);
		} else {
			tabController_->Show();
			std::shared_ptr<Widget> w;
			w = SharePtr(new MainWidget(L"Test 1"));
			w->setSelfHoldEnabled(true); 	// since we are passing ownership, allow object to hold itself where necessary
			w->setParent(tabController_);
			w = SharePtr(new MainWidget(L"Test 2"));
			w->setSelfHoldEnabled(true); 	// since we are passing ownership, allow object to hold itself where necessary
			w->setParent(tabController_);
			w = SharePtr(new MainWidget(L"Test 3.00000"));
			w->setSelfHoldEnabled(true); 	// since we are passing ownership, allow object to hold itself where necessary
			w->setParent(tabController_);
			w = SharePtr(new MainWidget(L"Test 4.5"));
			w->setSelfHoldEnabled(true); 	// since we are passing ownership, allow object to hold itself where necessary
			w->setParent(tabController_);
		}

		tabController_->Show();
		tabController_->setEventHandler(WidgetEventType::close, NewEventExt(*this, &Main::OnWidgetClose));
	}

	~Main() {
	}

private:
	std::shared_ptr<Widget> tabController_;
	int OnWidgetClose(Widget &sender, WidgetEventParams &params) {
		PostQuitMessage(0);
		std::fstream state("serialtest.state", std::ios::in | std::ios::out | std::ios::binary);
		if(!state.is_open()) {
			state.clear();
			state.open("serialtest.state", std::ios::out);
			state.close();
			state.open("serialtest.state", std::ios::in | std::ios::out | std::ios::binary);
		}
		tabController_->Serialize(state);
		state.flush();
		params.final = true;
		return 0;
	}
};

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	serializing::Serializable::RegisterSerializableClass<MainWidget>();
	std::shared_ptr<Main> mainClass (new Main());

	MessageBoxW(GetActiveWindow(),
			L"1. The window underneath should have five tabs,\n"
			 "   each one has edittest window in it",
			L"Instructions", MB_OK);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DWORD res = MessageBoxW(GetActiveWindow(), L"Was everything according to the instructions?",
			L"Test", MB_YESNO);
	if(res == IDNO)
		return 1;
	else if (res != IDYES) {
		res = GetLastError();
	}

	return (int) msg.wParam;
}

inline void MainWidget::RegisterFields() {
	RegisterField(text_, NewEvent(*this, &MainWidget::TextDeserialize));
	TabWidget::RegisterFields();
}

void MainWidget::Sizer() {
	input->setPosition(10_scaled, 10_scaled);
	input->setSize(getWidth() - 15_scaled, 25_scaled);

	output->setPosition(10_scaled, input->getPosition().y+input->getSize().cy + 5_scaled);
	output->setSize(getWidth() - 15_scaled, 25_scaled);
}

int MainWidget::onInputChange(Window& sender, const std::wstring& text) {
	output->setText(text);
	text_ = text;
	return 0;
}

int MainWidget::onWidgetReload(Widget& sender) {
	checkNLoadComponents();
	return 0;
}

MainWidget::MainWidget(const std::wstring &name)  :
		TabWidget(name, 100_scaled, 100_scaled, 500_scaled, 500_scaled, getWindowDefaultStyle()) {
	setOnWidgetReload(NewEvent(*this, &MainWidget::onWidgetReload));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &MainWidget::onGeometryChange));
}

MainWidget::MainWidget()  :
		TabWidget() {
	setOnWidgetReload(NewEvent(*this, &MainWidget::onWidgetReload));
	setEventHandler(WidgetEventType::geometryChange, NewEventExt(*this, &MainWidget::onGeometryChange));
}

void MainWidget::checkNLoadComponents() {
	input = std::unique_ptr<EditboxWnd>(new EditboxWnd(L"Text field 1", 0, 0, 0, 0, getWindow()));
	output = std::unique_ptr<EditboxWnd>(new EditboxWnd(L"Text field 2...", 0, 0, 0, 0, getWindow()));

	output->setReadonly(true);

	Sizer();
	if(!text_.empty()) {
		input->setText(text_);
		output->setText(text_);
	}
	input->Show();
	output->Show();

	input->setOnTextChange(NewEventExt(*this, &MainWidget::onInputChange));

}

int MainWidget::onGeometryChange(Widget& sender,
		WidgetEventParams& params) {
	Sizer();
	return 0;
}

int MainWidget::TextDeserialize(
		serializing::_internal::SFieldNotifying<std::wstring>& sender) {
	if(input)
		input->setText(text_);
	if(output)
		output->setText(text_);
	return 0;
}
