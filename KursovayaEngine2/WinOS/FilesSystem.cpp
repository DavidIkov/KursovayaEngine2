#include"FilesSystem.h"
#include"Tools/DebuggingTools.h"
#include<iostream>
#include"Tools/Time.h"


std::string FilesSystemNamespace::SaveFileToString(const wchar_t* filePath) {
	FilesSystemNamespace::OpenedFile file(filePath);
	std::string retStr;
	std::string	curStr;
	while (file.GetNextLine(curStr)) {
		retStr += curStr + '\n';
	}
	return retStr;
}
std::wstring FilesSystemNamespace::SaveFileToWString(const wchar_t* filePath) {
	FilesSystemNamespace::OpenedFileW file(filePath);
	std::wstring retStr;
	std::wstring	curStr;
	while (file.GetNextLine(curStr)) {
		retStr += curStr + L'\n';
	}
	return retStr;
}

bool FilesSystemNamespace::OpenedFile::GetNextLine(std::string& str) {
	return (bool)std::getline(FileStream, str);
}
void FilesSystemNamespace::OpenedFile::GoToStartOfStream() {
	FileStream.clear();
	FileStream.seekg(0, FileStream.beg);
}
FilesSystemNamespace::OpenedFile::OpenedFile(const wchar_t* filePath) :FilePath(filePath), StartTime(TimeNamespace::GetTime()) {

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
FilesSystemNamespace::OpenedFile::~OpenedFile() {
	std::wcout << L"Finished reading file \"" << FilePath << L"\" after " << TimeNamespace::GetTime() - StartTime << L"s\n";
	FileStream.close();
}


bool FilesSystemNamespace::OpenedFileW::GetNextLine(std::wstring& str) {
	return (bool)std::getline(FileStream, str);
}
void FilesSystemNamespace::OpenedFileW::GoToStartOfStream() {
	FileStream.clear();
	FileStream.seekg(0, FileStream.beg);
}
FilesSystemNamespace::OpenedFileW::OpenedFileW(const wchar_t* filePath) :FilePath(filePath), StartTime(TimeNamespace::GetTime()) {

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
FilesSystemNamespace::OpenedFileW::~OpenedFileW() {
	std::wcout << L"Finished reading file \"" << FilePath << L"\" after " << TimeNamespace::GetTime() - StartTime << L"s\n";
	FileStream.close();
}