#pragma once
#include"DLL.h"
#include"ErrorCodes.h"



namespace DebuggingTools {
	enum class ErrorTypes {
		Warning,//Warning error means that it can be "ignored" in some way without breaking logics of program
		Critical,//critical error means that it cant be "ignored", since if you will ignore it program may or will fall apart
	};

	inline bool IgnoreWarns = false;
	inline bool IgnoreErrors = false;

	struct Error {
		DebuggingTools::ErrorTypes ErrorType;
		const char* Message;
		KURSAVAYAENGINE2_CORE_ERRORS ErrorCode;
	};
	struct WideError {
		DebuggingTools::ErrorTypes ErrorType;
		const wchar_t* Message;
		KURSAVAYAENGINE2_CORE_ERRORS ErrorCode;
	};

	DLLTREATMENT void ManageTheError(const Error&& err);
	DLLTREATMENT void ManageTheError(const WideError&& err);
}