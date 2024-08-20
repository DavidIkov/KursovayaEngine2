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

	Vector<2> Position;
	Vector<2> Size;
	Vector<2> LocalOffset;

	ClickTrigger(Window* windPtr);
	~ClickTrigger();
	const EventClass& gLeftMouseButtonEvent();
	const EventClass& gRightMouseButtonEvent();
	const EventConnectionsHandlerClass& gEventsConsHandler();
};