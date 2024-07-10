#include"ReadFromFile.h"
#include<fstream>
#include"Tools/DebuggingTools.h"
#include"Tools/Time.h"
#include<iostream>

std::string ReadFromFile(const char* filepath) {
	
	TimePoint timeStart = Time::GetTimePoint();

	std::string result;
	std::ifstream fileTextStream;
	fileTextStream.open(filepath);
	if (fileTextStream.fail()) {
		std::string errMsg;
		errMsg += "Failed to open file \"";
		errMsg += filepath;
		errMsg += '\"';
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION});
	}
	std::string line;
	while (std::getline(fileTextStream,line))
		result += line + '\n';
	fileTextStream.close();

	std::cout << "Opened and saved file to string \"" << filepath << "\" in " << Time::GetDuration(timeStart, Time::GetTimePoint()) << " seconds" << std::endl;

	return result;
}