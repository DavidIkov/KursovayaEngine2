#pragma once
#include"DLL.h"
#include <string>
#include<vector>
#include"Tools/ErrorsSystem.h"

//copy pasted code, i fucking HATE WINDOWS API
namespace FileExplorerDialog {

    struct FailedToOpenFileExplorerDialog_Error :KE2::ErrorsSystemNamespace::ErrorBase {};

    struct DialogFilter {
        const wchar_t* VisualName;
        const wchar_t* FileType;
    };

    //the "*" format is ANY format
    DLLTREATMENT std::wstring OpenDialog(std::vector<DialogFilter> filters);
}