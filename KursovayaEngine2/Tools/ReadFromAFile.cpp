#include"ReadFromFile.h"
#include<fstream>
#include"Tools/DebuggingTools.h"

std::string ReadFromFile(const char* filepath) {
	std::string result;
	std::ifstream fileTextStream;
	fileTextStream.open(filepath);
	if (fileTextStream.fail()) {
		std::string errMsg;
		errMsg += "Failed to open file \"";
		errMsg += filepath;
		errMsg += '\"';
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_TO_OPEN_FILE});
	}
	std::string line;
	while (std::getline(fileTextStream,line))
		result += line + '\n';
	fileTextStream.close();
	return result;
}