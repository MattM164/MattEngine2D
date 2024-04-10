#pragma once
#include <windows.h>
#include <winuser.h>
#include <string>
using namespace std;

std::string wstringToString(const std::wstring& wstr);

string OpenFileExplorer() {

    OPENFILENAMEW ofn;       // common dialog box structure
    wchar_t szFile[260];      // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Display the Open dialog box
    if (GetOpenFileNameW(&ofn) == TRUE) {
        // User selected a file, you can use ofn.lpstrFile to get the selected file path
        // 
        //MessageBoxW(NULL, ofn.lpstrFile, L"Selected File", MB_OK);   //makes a message box that's cool
    }
    std::wstring selectedFilePath = ofn.lpstrFile;
    return wstringToString(selectedFilePath); //result

}

std::string wstringToString(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}