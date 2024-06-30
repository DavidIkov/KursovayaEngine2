#pragma once
#include"ErrorCodes.h"



namespace DebuggingTools {
	enum class ErrorTypes {
		Managable,//managable error means that it can be "ignored" in some way without breaking logics of program. Also known as "warning"
		Critical,//critical error means that it cant be "ignored", since if you will ignore it program may or will fall apart
	};

	struct Error {
		DebuggingTools::ErrorTypes ErrorType;
		const char* Message;
		KURSAVAYAENGINE2_CORE_ERRORS ErrorCode;
	};

	void ManageTheError(const Error&& err);
}