#include <iostream>﻿
#include <Windows.h>
using namespace std;

#define FILE_PATH L"D:/BSTU/5 sem/ОС/Лабы/9/Windows/OS09-01.txt"
#define READ_BYTES 420

string getFileName(wchar_t* filePath) {
    wstring ws(filePath);
    string filename(ws.begin(), ws.end());
    const size_t last_slash_idx = filename.find_last_of("\\/");
    if (string::npos != last_slash_idx)
        filename.erase(0, last_slash_idx + 1);
    return filename;
}

LPCWSTR getFileType(HANDLE file) {
    switch (GetFileType(file)) {
        case FILE_TYPE_UNKNOWN:
            return L"FILE_TYPE_UNKNOWN";
        case FILE_TYPE_DISK:
            return L"FILE_TYPE_DISK";
        case FILE_TYPE_CHAR:
            return L"FILE_TYPE_CHAR";
        case FILE_TYPE_PIPE:
            return L"FILE_TYPE_PIPE";
        case FILE_TYPE_REMOTE:
            return L"FILE_TYPE_REMOTE";
        default:
            return L"[ERROR]: WRITE FILE TYPE";
    }
}

BOOL printFileInfo(LPWSTR path) {
    HANDLE file = CreateFile(
        path,
        GENERIC_READ,
        NULL,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    SYSTEMTIME sysTime;
    BY_HANDLE_FILE_INFORMATION fi;
    BOOL fResult = GetFileInformationByHandle(file, &fi);
    if (fResult) {
        cout << "Имя файла:\t\t" << getFileName((wchar_t*)FILE_PATH);
        wcout << L"\nТип файла:\t\t" << getFileType(file);
        cout << "\nРазмер файла:\t\t" << fi.nFileSizeLow << " Б";
        FileTimeToSystemTime(&fi.ftCreationTime, &sysTime);
        cout << "\nВремя создания:\t\t" << sysTime.wDay << '.' << sysTime.wMonth << '.' << sysTime.wYear << " " << sysTime.wHour + 3 << '.' << sysTime.wMinute << '.' << sysTime.wSecond;
        FileTimeToSystemTime(&fi.ftLastWriteTime, &sysTime);
        cout << "\nВремя обновления:\t" << sysTime.wDay << '.' << sysTime.wMonth << '.' << sysTime.wYear << " " << sysTime.wHour + 3 << '.' << sysTime.wMinute << '.' << sysTime.wSecond;
    }
    CloseHandle(file);
    return true;
}

BOOL printFileTxt(LPWSTR path) {
    HANDLE file = CreateFile(
        path,
        GENERIC_READ,
        NULL,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    char buf[1024];
    DWORD countBytes = NULL;
    ZeroMemory(buf, sizeof(buf));
    BOOL b = ReadFile(
        file,
        &buf,
        READ_BYTES,
        &countBytes,
        NULL);
    if (!b)
        throw "[ERROR] Функция ReadFile вызвала исключение.";
    cout << "\n\n\tДанные из файла (" << countBytes << " Б)\n\n" << buf << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    LPWSTR path = (LPWSTR)FILE_PATH;
    cout << "\n=======================================\n\n";
    printFileInfo(path);
    cout << "\n\n=======================================";
    printFileTxt(path);
    cout << "\n=======================================\n";
}