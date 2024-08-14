#pragma once
#include"DLL.h"
#include <string>
#include<vector>

//copy pasted code, i fucking HATE WINDOWS API
namespace FileExplorerDialog {
    struct DialogFilter {
        const wchar_t* VisualName;
        const wchar_t* FileType;
    };

    //the "*" format is ANY format
    DLLTREATMENT std::wstring OpenDialog(std::vector<DialogFilter> filters);
}