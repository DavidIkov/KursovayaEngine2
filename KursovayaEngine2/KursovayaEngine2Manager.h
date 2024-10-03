#pragma once
#include"DLL.h"
#include"Tools/ErrorCodes.h"
#include"Tools/DebuggingTools.h"

#if !defined KE2_DLL
#if !defined KE2_User
#error you need to define a global KE2_User
#endif
#endif

DLLTREATMENT constexpr bool _IsKE2_InDebugConfig();
DLLTREATMENT constexpr bool _IsKE2_InEfficiencyConfig();
DLLTREATMENT constexpr bool _IsKE2_In_x86_Platform();
DLLTREATMENT constexpr bool _IsKE2_In_x64_Platform();

DLLTREATMENT KURSAVAYAENGINE2_CORE_ERRORS _InitializeKE2();

#if defined KE2_User
KURSAVAYAENGINE2_CORE_ERRORS InitializeKE2() {


	//checking that configs and platforms are same in project and a dll

#if !defined KE2_Debug and !defined KE2_Efficiency
#error you need to globally define KE2_Debug or KE2_Efficiency
#elif defined KE2_Debug and defined KE2_Efficiency
#error you have to define only one macro out of KE2_Debug and KE2_Efficiency
#elif defined KE2_Debug
	if (not _IsKE2_InDebugConfig()) 
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "project is using KE2 in debug config but dll is in efficiency config", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
#elif defined KE2_Efficiency
	if (not _IsKE2_InEfficiencyConfig()) 
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "project is using KE2 in efficiency config but dll is in debug config", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
#endif

#if !defined KE2_x64 and !defined KE2_x86
#error you need to globally define KE2_x64 or KE2_x86
#elif defined KE2_x64 and defined KE2_x86
#error you have to define only one macro out of KE2_x64 and KE2_x86
#elif defined KE2_x64
	if (not _IsKE2_In_x64_Platform()) 
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "project is using KE2 with x64 platform but dll is using x86 platform", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
#elif defined KE2_x86
	if (not _IsKE2_In_x86_Platform()) 
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical, "project is using KE2 with x86 platform but dll is using x64 platform", KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_IMPOSSIBLE_FUNCTION });
#endif

	return _InitializeKE2();

}
#endif
DLLTREATMENT void UninitializeKE2();
