#include"wchar_t_Operations.h"
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"Windows.h"


void char_to_wchar(const char* input, wchar_t** output) {
    int wchars_num = MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, 0);
    *output = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_ACP, 0, input, -1, *output, wchars_num);
}
void char_to_wchar(std::string* input, std::wstring* output) {
    wchar_t* wfpa = nullptr;
    char_to_wchar(input->c_str(), &wfpa);
    *output = wfpa;
    delete[] wfpa;
}

//void wchar_to_char(const wchar_t* input, char** output, unsigned int amountOfEndSymbolsForEnd) {
//    unsigned int len = 0;
//    while (true) {
//        bool endFound = true;
//        for (unsigned int i = 0; i < amountOfEndSymbolsForEnd; i++) {
//            endFound = *((char*)(input + len + i)) == '\0';
//            if (not endFound) break;
//        }
//        if (endFound) {
//            len += amountOfEndSymbolsForEnd;
//            break;
//        }
//        else {
//
//            /*check if character is wide. if its is, then next bit will be empty(\0),
//            if its not then i think theres no way to convert it to char*/
//            if (*((char*)(input + len) + 1) != '\0')
//                DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning,"Failed to properly convert wchar_t to char",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
//
//            len++;
//        }
//    }
//
//    *output = new char[len];
//
//    for (unsigned int i = 0; i < len; i++) {
//        (*output)[i] = *(char*)(input + i);
//    }
//}
//
//void char_to_wchar(const char* input, wchar_t** output, unsigned int amountOfEndSymbolsForEnd) {
//    unsigned int len = 0;
//    while (true) {
//        bool endFound = true;
//        for (unsigned int i = 0; i < amountOfEndSymbolsForEnd; i++) {
//            endFound = *(input + len + i) == '\0';
//            if (not endFound) break;
//        }
//        if (endFound) {
//            len += amountOfEndSymbolsForEnd;
//            break;
//        }
//        else len++;
//    }
//
//    *output = new wchar_t[len];
//
//    for (unsigned int i = 0; i < len; i++) {
//        *(char*)(*output + i) = input[i];
//        *((char*)(*output + i) + 1) = '\0';
//    }
//}