#include"UniqueKeysSystem.h"
#include"DebuggingTools.h"

UniqueKey UniqueKeysSystem::CreateKey() {
	if (FreeKeysAmount == 0) {
		KeysStates.push_back(true);
		FreeKeysLocations.push_back(0);
		return (UniqueKey)KeysStates.size();
;	}
	else {
		UniqueKey key = FreeKeysLocations[FreeKeysAmount - 1];
		KeysStates[key] = true;
		FreeKeysAmount--;
		return key + 1;
	}
}
bool UniqueKeysSystem::KeyExists(UniqueKey key) const {
	if (key > KeysStates.size()) return false;
	return KeysStates[key - 1];
}
void UniqueKeysSystem::RemoveKey(UniqueKey key) {
#if defined Debug
	if (key > KeysStates.size()) {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Managable, "Tried removing key that dosent exist", KURSAVAYAENGINE2_CORE_ERRORS::ATTEMPING_TO_REMOVE_NON_EXISTANT_KEY_FROM_KEYS_SYSTEM });
		return;
	} 
#endif
	KeysStates[key - 1] = false;
	FreeKeysLocations[FreeKeysAmount] = key - 1;
	FreeKeysAmount++;
}