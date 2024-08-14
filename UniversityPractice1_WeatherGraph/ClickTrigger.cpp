#include"ClickTrigger.h"
#include<iostream>


ClickTrigger::ClickTrigger(Window* windPtr) {
	WindPtr = windPtr;


	EventsConsHandler.ConnectToEvent(&windPtr->gMouseHandle().gMouseInputKeyEvent(MouseInputKeys::Left), [&](void* data) {
		LeftMouseButtonEvent.FireEvent(data);
		}, [&](void* pressedDown)->bool {
			Vector2 mousePos; WindPtr->gCursorPosition(&mousePos); mousePos /= WindPtr->gWindowSize() / 2;
			Vector2 PosOnScreen = Position - Size * LocalOffset;
			if (mousePos.gX() > PosOnScreen.gX() - Size.gX() and mousePos.gX() < PosOnScreen.gX() + Size.gX()
				and mousePos.gY() > PosOnScreen.gY() - Size.gY() and mousePos.gY() < PosOnScreen.gY() + Size.gY())
				return true;
			return false;
			});
	EventsConsHandler.ConnectToEvent(&windPtr->gMouseHandle().gMouseInputKeyEvent(MouseInputKeys::Right), [&](void* data) {
		RightMouseButtonEvent.FireEvent(data);
		}, [&](void* pressedDown)->bool {
			Vector2 mousePos; WindPtr->gCursorPosition(&mousePos); mousePos /= WindPtr->gWindowSize() / 2;
			Vector2 PosOnScreen = Position - Size * LocalOffset;
			if (mousePos.gX() > PosOnScreen.gX() - Size.gX() and mousePos.gX() < PosOnScreen.gX() + Size.gX()
				and mousePos.gY() > PosOnScreen.gY() - Size.gY() and mousePos.gY() < PosOnScreen.gY() + Size.gY())
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