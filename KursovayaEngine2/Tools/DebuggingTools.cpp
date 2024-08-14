#include"DebuggingTools.h"
#include<iostream>
#include"Windows.h"

void DebuggingTools::ManageTheError(const Error&& err) {
	if (err.ErrorType == DebuggingTools::ErrorTypes::Warning and not IgnoreWarns) {
		std::cout << "KURSOVAYAENGINE2 CORE WARNING: " << err.Message << " CODE: " << (unsigned int)err.ErrorCode << std::endl;
		MessageBeep(MB_OK);
#if defined _DEBUG
		std::cin.get();
#endif
	}
	else if (err.ErrorType == DebuggingTools::ErrorTypes::Critical and not IgnoreErrors) {
		std::cout << "KURSOVAYAENGINE2 CORE ERROR: " << err.Message << " CODE: " << (unsigned int)err.ErrorCode << std::endl;
		MessageBeep(MB_OK);
#if defined _DEBUG
		__debugbreak();
#endif
		std::cin.get();
		throw err.ErrorCode;
	}
}
void DebuggingTools::ManageTheError(const WideError&& err) {
	if (err.ErrorType == DebuggingTools::ErrorTypes::Warning and not IgnoreWarns) {
		std::wcout << L"KURSOVAYAENGINE2 CORE WARNING: " << err.Message << L" CODE: " << (unsigned int)err.ErrorCode << std::endl;
		MessageBeep(MB_OK);
#if defined _DEBUG
		std::cin.get();
#endif
	}
	else if (err.ErrorType == DebuggingTools::ErrorTypes::Critical and not IgnoreErrors) {
		std::wcout << L"KURSOVAYAENGINE2 CORE ERROR: " << err.Message << L" CODE: " << (unsigned int)err.ErrorCode << std::endl;
		MessageBeep(MB_OK);
#if defined _DEBUG
		__debugbreak();
#endif
		std::cin.get();
		throw err.ErrorCode;
	}
}