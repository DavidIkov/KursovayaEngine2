#include"Event.h"
#include"iostream"

void EventClass::FireEvent(void* data) {
	if (NeedBufferSwap) {
		ConnectionsFrontBuffer.ChangeCapacity(0);
		ConnectionsFrontBuffer.ChangeCapacity(ConnectionsBackBuffer.gLen());
		for (unsigned int i = 0; i < ConnectionsBackBuffer.gLen(); i++) {
			ConnectionsFrontBuffer.InsertAtIndex(ConnectionsFrontBuffer.gLen(), ConnectionsBackBuffer[i].Func, ConnectionsBackBuffer[i].CheckFunc);
			ConnectionsBackBuffer[i].IndInFrontBuffer = i + 1;
		}
		NeedBufferSwap = false;
	}
	EventIsFiring = true;
	for (unsigned int i = 0; i < ConnectionsFrontBuffer.gLen(); i++)
		ConnectionsFrontBuffer[i].CheckFuncPassed = (not ConnectionsFrontBuffer[i].Deleted) and ConnectionsFrontBuffer[i].CheckFunc(data);
	for (unsigned int i = 0; i < ConnectionsFrontBuffer.gLen(); i++)
		if (ConnectionsFrontBuffer[i].CheckFuncPassed) ConnectionsFrontBuffer[i].Func(data);

	
	EventIsFiring = false;
}

EventClass::~EventClass() {
	for (unsigned int i = 0; i < ConnectionsBackBuffer.gLen(); i++) {
		auto& cons = ConnectionsBackBuffer[i].EventConnectionsHandler->Connections;
		for (unsigned int ci = ConnectionsBackBuffer[i].ConnectionInd + 1; ci < cons.gLen(); ci++) {
			cons[ci].Event->ConnectionsBackBuffer[cons[ci].ConnectionInd].ConnectionInd--;
		}
		cons.RemoveAtIndex(ConnectionsBackBuffer[i].ConnectionInd);
	}
}

EventClass::ConnectionDataInFrontBufferClass::ConnectionDataInFrontBufferClass(std::function<void(void*)>& func, std::function<bool(void*)>& checkFunc) :Func(func), CheckFunc(checkFunc) { }

EventClass::ConnectionDataInBackBufferClass::ConnectionDataInBackBufferClass(std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc, float priority, const EventConnectionsHandlerClass* handler, unsigned int conInd) :
	Func(std::move(func)), CheckFunc(std::move(checkFunc)), Priority(priority), EventConnectionsHandler((EventConnectionsHandlerClass*)handler), ConnectionInd(conInd) { }





EventConnectionsHandlerClass::ConnectionDataClass::ConnectionDataClass(ConnectionID id, const EventClass* ev, unsigned int conInd) :
	ID(id), Event(ev), ConnectionInd(conInd) {}

EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::_ConnectToEvent
(const EventClass* ev, float priority, unsigned int priorityInsertInd, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
	
	Connections.InsertAtIndex(Connections.gLen(), (ConnectionID)IDCounter, ev, priorityInsertInd);

	ev->ConnectionsBackBuffer.InsertAtIndex(priorityInsertInd, std::move(func), std::move(checkFunc),
		priority, this, (unsigned int)Connections.gLen() - 1);

	ev->NeedBufferSwap = true;
	return IDCounter++;
}

EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func) const {
	return ConnectToEvent(ev, std::move(func), [](void*)->bool {return true; });
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
	return _ConnectToEvent(ev, (ev->ConnectionsBackBuffer.gLen() == 0) ? 0 : (ev->ConnectionsBackBuffer[ev->ConnectionsBackBuffer.gLen() - 1].Priority + 1),
		(unsigned int)ev->ConnectionsBackBuffer.gLen(), std::move(func), std::move(checkFunc));
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func) const {
	return ConnectToEvent(ev, priority, std::move(func), [](void*)->bool {return true; });
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {

	unsigned int ii = (unsigned int)ev->ConnectionsBackBuffer.gLen();
	for (unsigned int i = 0; i < ii; i++) if (ev->ConnectionsBackBuffer[i].Priority > priority) { ii = i; break; }

	return _ConnectToEvent(ev, priority, ii, std::move(func), std::move(checkFunc));
}
void EventConnectionsHandlerClass::RemoveConnection(const ConnectionID id) {
	for (unsigned int i = 0; i < Connections.gLen(); i++) {
		if (Connections[i].ID == id) {
			auto& econs = Connections[i].Event->ConnectionsBackBuffer;
			for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.gLen(); ci++)
				econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;
			for (unsigned int ci = i + 1; ci < Connections.gLen(); ci++)
				Connections[ci].Event->ConnectionsBackBuffer[Connections[ci].ConnectionInd].ConnectionInd--;
			
			if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
				Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

			econs.RemoveAtIndex(Connections[i].ConnectionInd);
			Connections.RemoveAtIndex(i);

			Connections[i].Event->ConnectionsBackBuffer = Connections[i].Event->ConnectionsBackBuffer;
			Connections[i].Event->NeedBufferSwap = true;

			return;
		}
	}

	KE2::ErrorsSystemNamespace::SendError << "attemping to remove connection with invalid id(it can be already deleted or just invalid)" >> ErrorsEnumWrapperStruct(ErrorsEnum::IncorrectID);
}

EventConnectionsHandlerClass::~EventConnectionsHandlerClass() {
	for (unsigned int i = 0; i < Connections.gLen(); i++) {

		auto& econs = Connections[i].Event->ConnectionsBackBuffer;
		
		for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.gLen(); ci++)
			econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;

		if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
			Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

		Connections[i].Event->NeedBufferSwap = true;
		econs.RemoveAtIndex(Connections[i].ConnectionInd); 

	}
}