#pragma once
#include"UniqueKeysSystem.h"
#include"functional"
#include"DebuggingTools.h"
template<typename...EventRespodDataTypes>
class Event {
	UniqueKeysSystem KeysSystemForConnections;
	struct Connection {//if Key is 0 then its a "temporary" connection, which cant be removed since it wont record to keys, it will remove automatically
		unsigned int FiresTillRemoval = 0;
		UniqueKey Key = 0;
		std::function<void(EventRespodDataTypes...)> Function;
	};
	std::vector<Connection> Connections;
public:

	//if FiresTillRemoval != 0 then this is "temporary" connection, it cant be removed since it wont record to keys, it will remove automatically
	UniqueKey Connect(std::function<void(EventRespodDataTypes...)> func, unsigned int FiresTillRemoval = 0) {
		UniqueKey key = (FiresTillRemoval == 0) ? KeysSystemForConnections.CreateKey() : 0;
		Connections.push_back({ FiresTillRemoval,key,func });
		return key;
	}
	void Remove(UniqueKey key) {
		if (key == 0) {
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "Tried removing connection to event with key = 0, which wont exist", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_REMOVE_CONNECTION_FROM_EVENT_WITH_INVALID_KEY });
			return;
		}
		if (not KeysSystemForConnections.KeyExists(key)) {
			DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "Tried removing connection to event with key which dosent exist (its not 0)", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_REMOVE_CONNECTION_FROM_EVENT_WITH_INVALID_KEY });
			return;
		}
		for (unsigned int i = 0; i < Connections.size(); i++) {
			if (Connections[i].Key == key) {
				Connections.erase(Connections.begin() + i);
				return;
			}
		}
	}
	void FireEvent(EventRespodDataTypes... eventData) {
		for (int i = (int)Connections.size()-1; i > -1; i--) {
			Connections[i].Function(eventData...);
			if (Connections[i].Key == 0) {
				if (Connections[i].FiresTillRemoval == 1) {
					Connections.erase(Connections.begin() + i);
				}
				else Connections[i].FiresTillRemoval--;
			}
		}
	}
};