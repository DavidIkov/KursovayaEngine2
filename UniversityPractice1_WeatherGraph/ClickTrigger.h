#pragma once
#include"Tools/Event.h"
#include"Windows/Window.h"
#include"Maths/Vector.h"
class ClickTrigger {
	Window* WindPtr;
private:
	EventClass LeftMouseButtonEvent;
	EventClass RightMouseButtonEvent;
	EventConnectionsHandlerClass EventsConsHandler;
public:

	Vector2F Position;
	Vector2F Size;
	Vector2F LocalOffset;

	ClickTrigger(Window* windPtr);
	~ClickTrigger();
	const EventClass& gLeftMouseButtonEvent();
	const EventClass& gRightMouseButtonEvent();
	const EventConnectionsHandlerClass& gEventsConsHandler();
};