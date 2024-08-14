#include"FileExplorerDialog.h"
#include <Windows.h>
#include <shobjidl.h>
#include"Tools/ErrorCodes.h"
#include"Tools/DebuggingTools.h"
#include"wchar_t_Operations.h"
#include"iostream"


std::wstring FileExplorerDialog::OpenDialog(std::vector<DialogFilter> filters)
{


    OPENFILENAME ofn;       // common dialog box structure
    wchar_t szFile[260];       // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);

    //example: L"Excel files 2003(*.txt)\0*.txt\0Excel files 2007(*.xlsx)\0*.xlsx\0"
    std::wstring FilterString;
    for (unsigned int i = 0; i < filters.size(); i++) {
        FilterString += filters[i].VisualName;
        FilterString += L"(*.";
        FilterString += filters[i].FileType;
        FilterString += L')';
        FilterString += L'\0';
        FilterString += L"*.";
        FilterString += filters[i].FileType;
        FilterString += L'\0';
    }

    ofn.lpstrFilter = FilterString.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ENABLESIZING;

    // Display the Open dialog box.
    if (GetOpenFileName(&ofn) == TRUE)
        return { szFile };
    else {
        DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Warning, "Failed to read selected file", KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
        return {};
    }
}