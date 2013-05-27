/*
 * WidgetEventParams.h
 *
 *  Created on: 29.10.2012
 *      Author: Morphe
 */

#ifndef WIDGETEVENTPARAMS_H_
#define WIDGETEVENTPARAMS_H_

#include <platform.h>
#include <set>
#include <string>

enum class WidgetEventType : UINT;

struct WidgetEventParams {
	WidgetEventType event;
	bool final;
	constexpr WidgetEventParams(WidgetEventType _event) : event(_event), final{false} {}
};

typedef struct _tagWINMESSAGE WinMessage_t;

struct WidgetWinMsgParams : public WidgetEventParams {
	WinMessage_t &winMsg;
	constexpr WidgetWinMsgParams(WidgetEventType _event, WinMessage_t &winMsg_) :
				WidgetEventParams(_event), winMsg(winMsg_) {}
};

struct WidgetMouseEventParams : public WidgetEventParams {
	POINT &mousePoint_;
	constexpr WidgetMouseEventParams(WidgetEventType _event, POINT &mousePoint) :
				WidgetEventParams(_event), mousePoint_(mousePoint) {}
};

namespace Drawing {
class Drawer;
}

struct WidgetDrawEventParams : public WidgetEventParams {
	Drawing::Drawer &drawer;
	constexpr WidgetDrawEventParams(WidgetEventType _event, Drawing::Drawer &_drawer) :
				WidgetEventParams(_event), drawer(_drawer) {}
};

class Widget;
// Since it is not clear from the name: this event holds reference
// to another widget, which is the cause for event
struct WidgetToWidgetEventParams : public WidgetEventParams {
	Widget &refWidget;
	constexpr WidgetToWidgetEventParams(WidgetEventType _event, Widget &_refWidget) :
			WidgetEventParams(_event), refWidget(_refWidget) {}
};


#endif /* WIDGETEVENTPARAMS_H_ */
