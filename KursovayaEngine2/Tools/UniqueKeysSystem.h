#pragma once
#include"DLL.h"
#include<vector>

typedef unsigned int UniqueKey;

//key "0" dont exist
class UniqueKeysSystem {
	std::vector<bool> KeysStates;

	std::vector<UniqueKey> FreeKeysLocations;
	unsigned int FreeKeysAmount = 0;

public:
	DLLTREATMENT UniqueKey CreateKey();
	DLLTREATMENT bool KeyExists(UniqueKey key) const;
	DLLTREATMENT void RemoveKey(UniqueKey key);
};