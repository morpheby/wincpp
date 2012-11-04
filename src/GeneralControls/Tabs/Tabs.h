/*
 * Tabs.h
 *
 *  Created on: 04.11.2012
 *      Author: Morphe
 */

#ifndef TABS_H_
#define TABS_H_

#include <Widget.h>
#include <ButtonWnd.h>
#include <unordered_set>

class Tabs: public Widget {
public:
	Tabs();
	~Tabs();
private:
	class TabButton : public ButtonWnd, public std::enable_shared_from_this<TabButton> {
	public:
		TabButton(std::shared_ptr<Widget> controlledWidget, int x, int y, int height, HWND parent);
		std::shared_ptr<Widget> getControlledWidget() const {
			return controlledWidget_;
		}

		bool isSelected() const {
			return selected_;
		}

		void setSelected(bool selected) {
			selected_ = selected;
		}

		std::shared_ptr<TabButton> getShared() {
			return shared_from_this();
		}

	protected:
		void PaintWindow(Drawing::Drawer &drawer) override;
	private:
		bool selected_;
		std::shared_ptr<Widget> controlledWidget_;
	};


	std::vector<std::shared_ptr<TabButton>> tabs_;
	std::shared_ptr<TabButton> selection_;

	int onChildAttached(Widget &sender, WidgetToWidgetEventParams &params);
	int onChildDetached(Widget &sender, WidgetToWidgetEventParams &params);
	int onBtnClicked(ButtonWnd &sender);
	int onGeometryChange(Widget &sender, WidgetEventParams &params);

	void UpdateTabButtons();
};

#endif /* TABS_H_ */
