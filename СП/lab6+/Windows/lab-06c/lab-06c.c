#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

#define MAX_BUFFER 1024

void PrintDirectoryContents(LPCTSTR sDir) {
    WIN32_FIND_DATA findFileData;
    TCHAR dirSpec[MAX_PATH];
    HANDLE hFind;
    _stprintf_s(dirSpec, MAX_PATH, _T("%s\\*"), sDir);
    hFind = FindFirstFile(dirSpec, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        _ftprintf(stderr, _T("Каталог не найден: %s\n"), sDir);
        return;
    }
    _tprintf(_T("Содержимое каталога:\n"));
    do {
        if (_tcscmp(findFileData.cFileName, _T(".")) != 0 && _tcscmp(findFileData.cFileName, _T("..")) != 0) {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                _tprintf(_T("[Каталог] %s\n"), findFileData.cFileName);
            else
                _tprintf(_T("[Файл] %s\n"), findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
}

void WatchDirectory(LPCTSTR sDir) {
    HANDLE hDir = CreateFile(
        sDir,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );
    if (hDir == INVALID_HANDLE_VALUE) {
        _ftprintf(stderr, _T("Не удалось открыть каталог для мониторинга: %s\n"), sDir);
        return;
    }

    char buffer[MAX_BUFFER];
    DWORD bytesReturned;
    _tprintf(_T("Начинаем мониторинг каталога: %s\n"), sDir);
    _tprintf(_T("Ожидание изменений в каталоге...\n"));
    while (1) {
        if (ReadDirectoryChangesW(
            hDir,
            buffer,
            MAX_BUFFER,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_CREATION,
            &bytesReturned,
            NULL,
            NULL
        )) {
            FILE_NOTIFY_INFORMATION* info = (FILE_NOTIFY_INFORMATION*)buffer;
            do {
                WCHAR fileName[MAX_PATH];
                size_t fileNameLength = info->FileNameLength / sizeof(WCHAR);
                wcsncpy_s(fileName, MAX_PATH, info->FileName, fileNameLength);
                fileName[fileNameLength] = L'\0';
                switch (info->Action) {
                    case FILE_ACTION_ADDED:
                        _tprintf(_T("Добавление\n"));
                        break;
                    case FILE_ACTION_REMOVED:
                        _tprintf(_T("Удаление\n"));
                        break;
                    case FILE_ACTION_MODIFIED:
                        _tprintf(_T("Изменение\n"));
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        _tprintf(_T(""));
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        _tprintf(_T("Переименование\n"));
                        break;
                    default:
                        _tprintf(_T("Неизвестное действие\n"));
                        break;
                }
                if (info->NextEntryOffset == 0)
                    break;
                info = (FILE_NOTIFY_INFORMATION*)((BYTE*)info + info->NextEntryOffset);
            } while (1);
        }
        else {
            _ftprintf(stderr, _T("Ошибка чтения изменений каталога.\n"));
            break;
        }
    }
    CloseHandle(hDir);
}

int _tmain(int argc, TCHAR* argv[]) {
    setlocale(LC_ALL, ".UTF-8");
    if (argc < 2) {
        _ftprintf(stderr, _T("Использование: %s <путь к каталогу>\n"), argv[0]);
        return 1;
    }
    LPCTSTR directoryPath = argv[1];
    DWORD attributes = GetFileAttributes(directoryPath);
    if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        _ftprintf(stderr, _T("Каталог не существует: %s\n"), directoryPath);
        return 1;
    }
    PrintDirectoryContents(directoryPath);
    WatchDirectory(directoryPath);
    return 0;
}