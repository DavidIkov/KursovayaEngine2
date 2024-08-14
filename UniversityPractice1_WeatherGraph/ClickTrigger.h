#pragma once
#include"Tools/Event.h"
#include"Windows/Window.h"
#include"Maths/Vector2.h"
class ClickTrigger {
	Window* WindPtr;
private:
	EventClass LeftMouseButtonEvent;
	EventClass RightMouseButtonEvent;
	EventConnectionsHandlerClass EventsConsHandler;
	int a = 123;
public:

	Vector2 Position;
	Vector2 Size;
	Vector2 LocalOffset;

	ClickTrigger(Window* windPtr);
	~ClickTrigger();
	const EventClass& gLeftMouseButtonEvent();
	const EventClass& gRightMouseButtonEvent();
	const EventConnectionsHandlerClass& gEventsConsHandler();
};