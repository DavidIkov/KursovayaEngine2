#include"ClickTrigger.h"
#include<iostream>


ClickTrigger::ClickTrigger(WindowClass* windPtr) {
	WindPtr = windPtr;


	EventsConsHandler.ConnectToEvent(&windPtr->gMouseHandle().gMousePressableKeyEvent(MouseClass::PressableKeysEnum::Left), [&](void* data) {
		LeftMouseButtonEvent.FireEvent(data);
		}, [&](void* pressedDown)->bool {
			Vector2F mousePos; mousePos = (Vector2F)WindPtr->gCursorPosition() / WindPtr->gWindowSize() * 2;
			Vector2F PosOnScreen = Position - Size * LocalOffset;
			if (mousePos[0] > PosOnScreen[0] - Size[0] and mousePos[0] < PosOnScreen[0] + Size[0]
				and mousePos[1] > PosOnScreen[1] - Size[1] and mousePos[1] < PosOnScreen[1] + Size[1])
				return true;
			return false;
			});
	EventsConsHandler.ConnectToEvent(&windPtr->gMouseHandle().gMousePressableKeyEvent(MouseClass::PressableKeysEnum::Right), [&](void* data) {
		RightMouseButtonEvent.FireEvent(data);
		}, [&](void* pressedDown)->bool {
			Vector2F mousePos; mousePos = (Vector2F)WindPtr->gCursorPosition() / WindPtr->gWindowSize() * 2;
			Vector2F PosOnScreen = Position - Size * LocalOffset;
			if (mousePos[0] > PosOnScreen[0] - Size[0] and mousePos[0] < PosOnScreen[0] + Size[0]
				and mousePos[1] > PosOnScreen[1] - Size[1] and mousePos[1] < PosOnScreen[1] + Size[1])
				return true;
			return false;
			});
}

ClickTrigger::~ClickTrigger() {

}
const EventClass& ClickTrigger::gLeftMouseButtonEvent() {
	return LeftMouseButtonEvent;
}
const EventClass& ClickTrigger::gRightMouseButtonEvent() {
	return RightMouseButtonEvent;
}
const EventConnectionsHandlerClass& ClickTrigger::gEventsConsHandler() {
	return EventsConsHandler;
}