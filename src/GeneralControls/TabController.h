/*
 * TabController.h
 *
 *  Created on: 23.11.2012
 *      Author: Morphe
 */

#ifndef TABCONTROLLER_H_
#define TABCONTROLLER_H_

namespace Tabs {
class TabController;
}

#include <Widget.h>
#include "TabWidget.h"
#include <vector>

namespace Tabs {

class TabButton;

class TabController: public Widget {
public:
	TabController();
	TabController(Widget &parent);
	TabController(WidgetStyle style);
	TabController(WidgetStyle style, Widget &parent);
	TabController(const std::wstring &name);
	TabController(const std::wstring &name, Widget &parent);
	TabController(const std::wstring &name, WidgetStyle style);
	TabController(const std::wstring &name, WidgetStyle style, Widget &parent);
	TabController(int x, int y, int width, int height);
	TabController(int x, int y, int width, int height, Widget &parent);
	TabController(int x, int y, int width, int height, WidgetStyle style);
	TabController(int x, int y, int width, int height, WidgetStyle style, Widget &parent);
	TabController(const std::wstring &name, int x, int y, int width, int height);
	TabController(const std::wstring &name, int x, int y, int width, int height,
			Widget &parent);
	TabController(const std::wstring &name, int x, int y, int width, int height,
			WidgetStyle style);
	TabController(const std::wstring &name, int x, int y, int width, int height,
			WidgetStyle style, Widget &parent);
	~TabController();

protected:
	void DrawWindow(Drawing::Drawer &drawer) override;
	void widgetReload() override;
private:
	static class TabPool tabPool_;

	bool mouseCaptured_ {false};
	bool btnMouseDown_ {false};
	bool registered_ {false};
	bool firstInversion_{true};
	POINT mouseMovePos_{0,0};
	std::vector<std::shared_ptr<TabButton>> tabs_;
	std::shared_ptr<TabWidget> selection_;
	std::shared_ptr<TabButton> selectionBtn_;

	void init();

	void transferCapture(std::shared_ptr<TabWidget> movingTab);

	int onChildAttached(Widget& sender, WidgetEventParams& params);
	int onChildDetach(Widget& sender, WidgetEventParams& params);
	int onGeometryChange(Widget& sender, WidgetEventParams& params);
	int onBtnClicked(Window &sender, WinMessage_t &msg);
	int onBtnMouseReleased(Window &sender, WinMessage_t &msg);
	int onBtnMouseMove(Window &sender, WinMessage_t &msg);
	int onCloseBtnClicked(Window &sender);

	int onCloseInternal(Widget &sender, WidgetEventParams &params);

	int onMouseMove(Widget& sender, WidgetEventParams& params);
	int onMouseLBtnUp(Widget& sender, WidgetEventParams& params);

	void addTabBtn(const std::wstring &name);
	void removeTab(std::shared_ptr<TabButton> tab);
	void selectTab(std::shared_ptr<TabButton> tab);

	void UpdateTabButtons();
	void drawSquareInv(std::shared_ptr<Drawing::Drawer> drawerPtr);
};

} /* namespace Tabs */
#endif /* TABCONTROLLER_H_ */
