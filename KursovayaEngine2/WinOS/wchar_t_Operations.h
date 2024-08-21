#pragma once
#include"DLL.h"
#include<string>

DLLTREATMENT void char_to_wchar(unsigned int codeTable, const char* input, wchar_t** output);
DLLTREATMENT void char_to_wchar(unsigned int codeTable, const std::string& input, std::wstring& output);

DLLTREATMENT void wchar_to_char(unsigned int codeTable, const wchar_t* input, char** output);
DLLTREATMENT void wchar_to_char(unsigned int codeTable, const std::wstring& input, std::string& output);