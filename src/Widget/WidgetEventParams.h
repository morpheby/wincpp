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
	constexpr WidgetEventParams(WidgetEventType _event) : event(_event) {}
};

typedef struct _tagWINMESSAGE WinMessage_t;

struct WidgetWinMsgParams : public WidgetEventParams {
	WinMessage_t &winMsg;
	constexpr WidgetWinMsgParams(WidgetEventType _event, WinMessage_t &winMsg_) :
				WidgetEventParams(_event), winMsg(winMsg_) {}
};

namespace Drawing {
class Drawer;
}

struct WidgetDrawEventParams : public WidgetEventParams {
	Drawing::Drawer &drawer;
	constexpr WidgetDrawEventParams(WidgetEventType _event, Drawing::Drawer &_drawer) :
				WidgetEventParams(_event), drawer(_drawer) {}
};


#endif /* WIDGETEVENTPARAMS_H_ */
