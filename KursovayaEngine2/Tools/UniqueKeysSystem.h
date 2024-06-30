#pragma once
#include<vector>

typedef unsigned int UniqueKey;

//key "0" dont exist
class UniqueKeysSystem {
	std::vector<bool> KeysStates;

	std::vector<UniqueKey> FreeKeysLocations;
	unsigned int FreeKeysAmount = 0;

public:
	UniqueKey CreateKey();
	bool KeyExists(UniqueKey key) const;
	void RemoveKey(UniqueKey key);
};