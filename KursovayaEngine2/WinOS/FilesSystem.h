#pragma once
#include"DLL.h"
#include<string>
#include<fstream>

namespace FilesSystem {
	DLLTREATMENT std::string SaveFileToString(const wchar_t* filePath);
	DLLTREATMENT std::wstring SaveFileToWString(const wchar_t* filePath);


	class OpenedFile {
		const wchar_t* FilePath;
		float StartTime;
		std::ifstream FileStream;
	public:

		DLLTREATMENT bool GetNextLine(std::string& str);
		DLLTREATMENT void GoToStartOfStream();

		DLLTREATMENT OpenedFile(const wchar_t* filePath);
		DLLTREATMENT ~OpenedFile();
	};
	class OpenedFileW {
		const wchar_t* FilePath;
		float StartTime;
		std::wifstream FileStream;
	public:

		DLLTREATMENT bool GetNextLine(std::wstring& str);
		DLLTREATMENT void GoToStartOfStream();

		DLLTREATMENT OpenedFileW(const char* filePath);
		DLLTREATMENT OpenedFileW(const wchar_t* filePath);
		DLLTREATMENT ~OpenedFileW();

	};
}