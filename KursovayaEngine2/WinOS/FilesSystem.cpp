#include"FilesSystem.h"
#include"Tools/DebuggingTools.h"
#include<iostream>
#include"Tools/Time.h"


std::string FilesSystem::SaveFileToString(const wchar_t* filePath) {
	FilesSystem::OpenedFile file(filePath);
	std::string retStr;
	std::string	curStr;
	while (file.GetNextLine(curStr)) {
		retStr += curStr + '\n';
	}
	return retStr;
}
std::wstring FilesSystem::SaveFileToWString(const wchar_t* filePath) {
	FilesSystem::OpenedFileW file(filePath);
	std::wstring retStr;
	std::wstring	curStr;
	while (file.GetNextLine(curStr)) {
		retStr += curStr + L'\n';
	}
	return retStr;
}

bool FilesSystem::OpenedFile::GetNextLine(std::string& str) {
	return (bool)std::getline(FileStream, str);
}
void FilesSystem::OpenedFile::GoToStartOfStream() {
	FileStream.clear();
	FileStream.seekg(0, FileStream.beg);
}
FilesSystem::OpenedFile::OpenedFile(const wchar_t* filePath) :FilePath(filePath), StartTime(Time::GetTime()) {

	FileStream.open(filePath);
	if (FileStream.fail()) {
		std::wstring errMsg;
		errMsg += L"Failed to open file \"";
		errMsg += filePath;
		errMsg += L'\"';
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
	}

	std::wcout << L"Started reading file \"" << filePath << L"\"...\n";

}
FilesSystem::OpenedFile::~OpenedFile() {
	std::wcout << L"Finished reading file \"" << FilePath << L"\" after " << Time::GetTime() - StartTime << L"s\n";
	FileStream.close();
}


bool FilesSystem::OpenedFileW::GetNextLine(std::wstring& str) {
	return (bool)std::getline(FileStream, str);
}
void FilesSystem::OpenedFileW::GoToStartOfStream() {
	FileStream.clear();
	FileStream.seekg(0, FileStream.beg);
}
FilesSystem::OpenedFileW::OpenedFileW(const wchar_t* filePath) :FilePath(filePath), StartTime(Time::GetTime()) {

	FileStream.open(filePath);
	if (FileStream.fail()) {
		std::wstring errMsg;
		errMsg += L"Failed to open file \"";
		errMsg += filePath;
		errMsg += L'\"';
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
	}

	std::wcout << L"Started reading file \"" << filePath << L"\"...\n";

}
FilesSystem::OpenedFileW::~OpenedFileW() {
	std::wcout << L"Finished reading file \"" << FilePath << L"\" after " << Time::GetTime() - StartTime << L"s\n";
	FileStream.close();
}