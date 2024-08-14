#pragma once
#include"DLL.h"
#include<string>

//void wchar_to_char(const wchar_t* input, char** output, unsigned int amountOfEndSymbolsForEnd = 1);
//void char_to_wchar(const char* input, wchar_t** output, unsigned int amountOfEndSymbolsForEnd = 1);

DLLTREATMENT void char_to_wchar(const char* input, wchar_t** output);
DLLTREATMENT void char_to_wchar(std::string* input, std::wstring* output);