#include"DebuggingTools.h"
#include<iostream>

void DebuggingTools::ManageTheError(const Error&& err) {
	if (err.ErrorType == DebuggingTools::ErrorTypes::Warning) {
		std::cout << "KURSOVAYAENGINE2 CORE WARNING: " << err.Message << " CODE: " << (unsigned int)err.ErrorCode << std::endl;
#if defined _DEBUG
		std::cin.get();
#endif
	}
	else if (err.ErrorType == DebuggingTools::ErrorTypes::Critical) {
		std::cout << "KURSOVAYAENGINE2 CORE ERROR: " << err.Message << " CODE: " << (unsigned int)err.ErrorCode << std::endl;
		std::cin.get();
		throw err.ErrorCode;
	}
}