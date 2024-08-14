#include"Event.h"
#include"DebuggingTools.h"
#include"iostream"

void EventClass::FireEvent(void* data) {
	if (NeedBufferSwap) {
		ConnectionsFrontBuffer.clear();
		ConnectionsFrontBuffer.reserve(ConnectionsBackBuffer.size());
		for (unsigned int i = 0; i < ConnectionsBackBuffer.size(); i++) {
			ConnectionsFrontBuffer.emplace_back(ConnectionsBackBuffer[i].Func, ConnectionsBackBuffer[i].CheckFunc);
			ConnectionsBackBuffer[i].IndInFrontBuffer = i + 1;
		}
		NeedBufferSwap = false;
	}
	EventIsFiring = true;
	for (unsigned int i = 0; i < ConnectionsFrontBuffer.size(); i++)
		ConnectionsFrontBuffer[i].CheckFuncPassed = (not ConnectionsFrontBuffer[i].Deleted) and ConnectionsFrontBuffer[i].CheckFunc(data);
	for (unsigned int i = 0; i < ConnectionsFrontBuffer.size(); i++)
		if (ConnectionsFrontBuffer[i].CheckFuncPassed) ConnectionsFrontBuffer[i].Func(data);

	
	EventIsFiring = false;
}

EventClass::~EventClass() {
	for (unsigned int i = 0; i < ConnectionsBackBuffer.size(); i++) {
		auto& cons = ConnectionsBackBuffer[i].EventConnectionsHandler->Connections;
		for (unsigned int ci = ConnectionsBackBuffer[i].ConnectionInd + 1; ci < cons.size(); ci++) {
			cons[ci].Event->ConnectionsBackBuffer[cons[ci].ConnectionInd].ConnectionInd--;
		}
		cons.erase(cons.begin() + ConnectionsBackBuffer[i].ConnectionInd);
	}
}

EventClass::ConnectionDataInFrontBufferClass::ConnectionDataInFrontBufferClass(std::function<void(void*)>& func, std::function<bool(void*)>& checkFunc) :Func(func), CheckFunc(checkFunc) { }

EventClass::ConnectionDataInBackBufferClass::ConnectionDataInBackBufferClass(std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc, float priority, const EventConnectionsHandlerClass* handler, unsigned int conInd) :
	Func(std::move(func)), CheckFunc(std::move(checkFunc)), Priority(priority), EventConnectionsHandler((EventConnectionsHandlerClass*)handler), ConnectionInd(conInd) { }





EventConnectionsHandlerClass::ConnectionDataClass::ConnectionDataClass(ConnectionID id, const EventClass* ev, unsigned int conInd) :
	ID(id), Event(ev), ConnectionInd(conInd) {}

EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::_ConnectToEvent
(const EventClass* ev, float priority, unsigned int priorityInsertInd, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
	
	Connections.emplace_back((ConnectionID)IDCounter, ev, priorityInsertInd);

	ev->ConnectionsBackBuffer.emplace(ev->ConnectionsBackBuffer.begin() + priorityInsertInd, std::move(func), std::move(checkFunc),
		priority, this, (unsigned int)Connections.size() - 1);

	ev->NeedBufferSwap = true;
	return IDCounter++;
}

EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func) const {
	return ConnectToEvent(ev, std::move(func), [](void*)->bool {return true; });
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {
	return _ConnectToEvent(ev, (ev->ConnectionsBackBuffer.size() == 0) ? 0 : (ev->ConnectionsBackBuffer[ev->ConnectionsBackBuffer.size() - 1].Priority + 1),
		(unsigned int)ev->ConnectionsBackBuffer.size(), std::move(func), std::move(checkFunc));
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func) const {
	return ConnectToEvent(ev, priority, std::move(func), [](void*)->bool {return true; });
}
EventConnectionsHandlerClass::ConnectionID EventConnectionsHandlerClass::ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const {

	unsigned int ii = (unsigned int)ev->ConnectionsBackBuffer.size();
	for (unsigned int i = 0; i < ii; i++) if (ev->ConnectionsBackBuffer[i].Priority > priority) { ii = i; break; }

	return _ConnectToEvent(ev, priority, ii, std::move(func), std::move(checkFunc));
}
void EventConnectionsHandlerClass::RemoveConnection(const ConnectionID id) {
	for (unsigned int i = 0; i < Connections.size(); i++) {
		if (Connections[i].ID == id) {
			auto& econs = Connections[i].Event->ConnectionsBackBuffer;
			for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.size(); ci++)
				econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;
			for (unsigned int ci = i + 1; ci < Connections.size(); ci++)
				Connections[ci].Event->ConnectionsBackBuffer[Connections[ci].ConnectionInd].ConnectionInd--;
			
			if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
				Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

			econs.erase(econs.begin() + Connections[i].ConnectionInd);
			Connections.erase(Connections.begin() + i);

			Connections[i].Event->ConnectionsBackBuffer = Connections[i].Event->ConnectionsBackBuffer;
			Connections[i].Event->NeedBufferSwap = true;

			return;
		}
	}

	DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "attemping to remove connection with invalid id(it can be already deleted or just invalid)", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
}

EventConnectionsHandlerClass::~EventConnectionsHandlerClass() {
	for (unsigned int i = 0; i < Connections.size(); i++) {

		auto& econs = Connections[i].Event->ConnectionsBackBuffer;
		
		for (unsigned int ci = Connections[i].ConnectionInd + 1; ci < econs.size(); ci++)
			econs[ci].EventConnectionsHandler->Connections[econs[ci].ConnectionInd].ConnectionInd--;

		if (econs[Connections[i].ConnectionInd].IndInFrontBuffer != 0)
			Connections[i].Event->ConnectionsFrontBuffer[econs[Connections[i].ConnectionInd].IndInFrontBuffer - 1].Deleted = true;

		Connections[i].Event->NeedBufferSwap = true;
		econs.erase(econs.begin() + Connections[i].ConnectionInd); 

	}
}