#pragma once
#include"DLL.h"
#include"DynArr.h"
#include"functional"
#include"ErrorsSystem.h"

class EventConnectionsHandlerClass;

class EventClass {
	friend class EventConnectionsHandlerClass;

	mutable bool NeedBufferSwap = false;

	bool EventIsFiring = false;

	struct ConnectionDataInFrontBufferClass {
		std::function<void(void*)> Func;
		std::function<bool(void*)> CheckFunc;
		bool CheckFuncPassed;

		bool Deleted = false;

		ConnectionDataInFrontBufferClass(std::function<void(void*)>& func, std::function<bool(void*)>& checkFunc);
	};
	mutable DynArr<ConnectionDataInFrontBufferClass> ConnectionsFrontBuffer;
	struct ConnectionDataInBackBufferClass {
		std::function<void(void*)> Func;
		std::function<bool(void*)> CheckFunc;

		float Priority;

		EventConnectionsHandlerClass* EventConnectionsHandler;
		unsigned int ConnectionInd;

		unsigned int IndInFrontBuffer = 0;//+1 so if its = 0 then its invalid

		ConnectionDataInBackBufferClass(std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc, float priority, const EventConnectionsHandlerClass* handler, unsigned int conInd);
	};
	mutable DynArr<ConnectionDataInBackBufferClass> ConnectionsBackBuffer;
public:
	DLLTREATMENT void FireEvent(void* data);

	DLLTREATMENT ~EventClass();
};

class EventConnectionsHandlerClass {
	friend class EventClass;

	mutable unsigned int IDCounter = 0;

public:
	typedef unsigned int ConnectionID;
private:

	struct ConnectionDataClass {
		ConnectionID ID;

		const EventClass* Event;
		unsigned int ConnectionInd;

		ConnectionDataClass(ConnectionID id, const EventClass* ev, unsigned int conInd);
	};
	mutable DynArr<ConnectionDataClass> Connections;

private: ConnectionID _ConnectToEvent(const EventClass* ev, float priority, unsigned int priorityInsertInd, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
public:

	struct ErrorsEnumWrapperStruct :KE2::ErrorsSystemNamespace::ErrorBase {
		enum ErrorsEnum {
			IncorrectID,
		};
		ErrorsEnum Error;
		inline ErrorsEnumWrapperStruct(ErrorsEnum error) :Error(error) {};
	}; using ErrorsEnum = ErrorsEnumWrapperStruct; using AnyError = ErrorsEnumWrapperStruct;

	DLLTREATMENT ConnectionID ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func) const;
	DLLTREATMENT ConnectionID ConnectToEvent(const EventClass* ev, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
	DLLTREATMENT ConnectionID ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func) const;
	DLLTREATMENT ConnectionID ConnectToEvent(const EventClass* ev, float priority, std::function<void(void*)>&& func, std::function<bool(void*)>&& checkFunc) const;
	DLLTREATMENT void RemoveConnection(const ConnectionID id);
	DLLTREATMENT ~EventConnectionsHandlerClass();
};