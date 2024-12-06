#include <iostream>
#include <cstdlib>
#include "Windows.h"
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#define FILE_PATH L"D:/BSTU/5 sem/ОС/Лабы/9/Windows/OS09-01.txt"
#define DIR_PATH L"D:/BSTU/5 sem/ОС/Лабы/9/Windows"

int rowC = 0;

BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec) {
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    LPWSTR path = (LPWSTR)DIR_PATH;
    char* cFileName = new char[wcslen(FileName) * sizeof(char) + 1];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, cFileName, sizeof(cFileName), FileName, _TRUNCATE);
    int i = 0;
    int lenName = 0;
    try {
        HANDLE notif = FindFirstChangeNotification(path, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        DWORD err = GetLastError();
        clock_t t1 = clock();
        clock_t t2 = clock();
        DWORD dwWaitStatus;
        printf("\nНачато наблюдение за файлом (timestamp %d)", t1);
        while (true) {
            dwWaitStatus = WaitForSingleObject(notif, mlsec);
            switch (dwWaitStatus) {
                case WAIT_OBJECT_0: {
                    if (FindNextChangeNotification(notif) == FALSE)
                        break;
                    else {
                        int position = 0;
                        int rowCount = 0;
                        HANDLE of = CreateFile(
                            FileName,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                        if (of == INVALID_HANDLE_VALUE) {
                            CloseHandle(of);
                            throw "[ERROR] Open file failed";
                        }
                        if (GetFileSizeEx(of, fileSize)) {
                            char* buf = new char[(fileSize->QuadPart + 1) * sizeof(char)];
                            ZeroMemory(buf, (fileSize->QuadPart + 1) * sizeof(char));
                            DWORD n = NULL;
                            if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
                                while (buf[position++] != '\0')
                                    if (buf[position] == '\n')
                                        rowCount++;
                        }
                        if (rowC != rowCount) {
                            printf("\n\nСтрок: %d", rowCount);
                            rowC = rowCount;
                        }
                        CloseHandle(of);
                    }
                }
            }
            t2 = clock();
            if (t2 - t1 > mlsec)
                break;
        }
        CloseHandle(notif);
        printf("\n\nЗакончено наблюдение за файлом (timestamp %d)\n", t2);
    }
    catch (const char* err) {
        cout << "[ERROR] " << err << "\n";
        return false;
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    LPWSTR fileName = (LPWSTR)FILE_PATH;
    printWatchRowFileTxt(fileName, 30000);
}