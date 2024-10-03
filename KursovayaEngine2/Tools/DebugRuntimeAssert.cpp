#include"DebugRuntimeAssert.h"

bool DebugRuntimeAssert(DebuggingTools::ErrorTypes errorType, bool correctBehaviour, const char* message, KURSAVAYAENGINE2_CORE_ERRORS errorCode) {
#if defined KE2_Debug
	if (not correctBehaviour) {
		DebuggingTools::ManageTheError({ errorType,message,errorCode }); 
		return true;
	}
#endif
	return false;
}
bool DebugRuntimeAssert(DebuggingTools::ErrorTypes errorType, bool correctBehaviour, const wchar_t* message, KURSAVAYAENGINE2_CORE_ERRORS errorCode){
#if defined KE2_Debug
	if (not correctBehaviour) {
		DebuggingTools::ManageTheError({ errorType,message,errorCode }); 
		return true;
	}
#endif
	return false;
}

