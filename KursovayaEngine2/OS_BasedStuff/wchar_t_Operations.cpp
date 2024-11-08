#include"wchar_t_Operations.h"
#include"Windows.h"


void char_to_wchar(unsigned int codeTable, const char* input, wchar_t** output) {
    int wchars_num = MultiByteToWideChar(codeTable, 0, input, -1, NULL, 0);
    *output = new wchar_t[wchars_num];
    MultiByteToWideChar(codeTable, 0, input, -1, *output, wchars_num);
}
void char_to_wchar(unsigned int codeTable,const std::string& input, std::wstring& output) {
    wchar_t* wp = nullptr;
    char_to_wchar(codeTable, input.c_str(), &wp);
    output = wp;
    delete[] wp;
}

void wchar_to_char(unsigned int codeTable, const wchar_t* input, char** output) {
    int chars_num = WideCharToMultiByte(codeTable, 0, input, -1, NULL, 0, NULL, NULL);
    *output = new char[chars_num];
    WideCharToMultiByte(codeTable, 0, input, -1, *output, chars_num, NULL, NULL);
}
void wchar_to_char(unsigned int codeTable,const std::wstring& input, std::string& output) {
    char* cp = nullptr;
    wchar_to_char(codeTable, input.c_str(), &cp);
    output = cp;
    delete[] cp;
}