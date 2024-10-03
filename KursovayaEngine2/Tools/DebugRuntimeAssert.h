#pragma once
#include"DLL.h"
#include"DebuggingTools.h"

//returns if correct behaviour was not met
DLLTREATMENT bool DebugRuntimeAssert(DebuggingTools::ErrorTypes errorType, bool correctBehaviour, const char* message, KURSAVAYAENGINE2_CORE_ERRORS errorCode);

//returns if correct behaviour was not met
DLLTREATMENT bool DebugRuntimeAssert(DebuggingTools::ErrorTypes errorType, bool correctBehaviour, const wchar_t* message, KURSAVAYAENGINE2_CORE_ERRORS errorCode);


