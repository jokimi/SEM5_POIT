#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <Shlwapi.h>
#include <tchar.h>

#define BUFFER_SIZE 1024

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle = NULL;
HANDLE directoryHandle = NULL;
FILE* srvLog = NULL;
FILE* dirLog = NULL;

char serviceName[] = "service-09";
char logDirectory[MAX_PATH];
char watchDirectory[MAX_PATH];
int isPaused = 0;
volatile int shouldStop = 0;
char dirLogPath[MAX_PATH];

void GetCurrentDatetime(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d_%H-%M-%S", tm_info);
}

void CreateNestedDirectories(const char* path) {
    char tempPath[MAX_PATH];
    char* p = NULL;
    size_t len;
    strncpy(tempPath, path, MAX_PATH);
    len = strlen(tempPath);
    if (tempPath[len - 1] == '\\' || tempPath[len - 1] == '/')
        tempPath[len - 1] = '\0';
    for (p = tempPath + 1; *p; p++) {
        if (*p == '\\' || *p == '/') {
            *p = '\0';
            _mkdir(tempPath);
            *p = '\\';
        }
    }
    _mkdir(tempPath); // Создаем финальный каталог
}

void LogMessage(FILE* logFile, const char* message) {
    if (logFile == NULL)
        return;
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[10];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);
    fprintf(logFile, "[%s] %s\n", timestamp, message);
    fflush(logFile);
}

void UpdateServiceStatus(DWORD state, DWORD exitCode) {
    serviceStatus.dwCurrentState = state;
    serviceStatus.dwWin32ExitCode = exitCode;
    serviceStatus.dwWaitHint = 0;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

const char* GetEventName(DWORD action) {
    switch (action) {
    case FILE_ACTION_ADDED:
        return "Added";
    case FILE_ACTION_REMOVED:
        return "Removed";
    case FILE_ACTION_MODIFIED:
        return "Modified";
    case FILE_ACTION_RENAMED_OLD_NAME:
        return "Renamed (Old Name)";
    case FILE_ACTION_RENAMED_NEW_NAME:
        return "Renamed (New Name)";
    default:
        return "Unknown Action";
    }
}

void PrintDirectoryContents(LPCTSTR sDir, FILE* logFile) {
    WIN32_FIND_DATA findFileData;
    TCHAR dirSpec[MAX_PATH];
    HANDLE hFind;

    sprintf(dirSpec, "%s\\*", sDir);
    hFind = FindFirstFile(dirSpec, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        LogMessage(logFile, "Error: Directory not found.");
        return;
    }
    LogMessage(logFile, "Directory contents:");
    do {
        if (_tcscmp(findFileData.cFileName, _T(".")) != 0 && _tcscmp(findFileData.cFileName, _T("..")) != 0) {
            TCHAR entry[MAX_PATH];
            sprintf(entry, "%s\\%s", sDir, findFileData.cFileName);

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                char logEntry[MAX_PATH];
                sprintf(logEntry, "[Directory] %s", entry);
                LogMessage(logFile, logEntry);
            }
            else {
                char logEntry[MAX_PATH];
                sprintf(logEntry, "[File] %s", entry);
                LogMessage(logFile, logEntry);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void WatchDirectoryChanges(HANDLE directoryHandle, FILE* dirLog) {
    char buffer[BUFFER_SIZE];
    DWORD bytesReturned;
    LogMessage(dirLog, "Starting directory monitoring...");
    LogMessage(dirLog, "");
    LogMessage(dirLog, "Initial directory contents:");
    PrintDirectoryContents(watchDirectory, dirLog);
    LogMessage(dirLog, "");
    LogMessage(dirLog, "Waiting for directory changes...");
    while (!shouldStop) {
        if (isPaused) {
            Sleep(10000);
            continue;
        }
        if (ReadDirectoryChangesW(directoryHandle, buffer, sizeof(buffer), TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, NULL, NULL)) {
            FILE_NOTIFY_INFORMATION* notify = (FILE_NOTIFY_INFORMATION*)buffer;
            do {
                int fileNameLength = notify->FileNameLength / sizeof(WCHAR);
                if (fileNameLength >= MAX_PATH) {
                    LogMessage(dirLog, "Error: File name too long.");
                    break;
                }
                char fileNameUtf8[MAX_PATH];
                int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, notify->FileName, fileNameLength, fileNameUtf8, MAX_PATH - 1, NULL, NULL);
                if (bytesConverted > 0) {
                    fileNameUtf8[bytesConverted] = '\0'; // Завершение строки
                }
                else {
                    sprintf(fileNameUtf8, "Error converting filename (code %lu)", GetLastError());
                }
                char logMessage[512];
                sprintf(logMessage, "Change detected: Action: %s, File/Directory: %s", GetEventName(notify->Action), fileNameUtf8);
                LogMessage(dirLog, logMessage);
                LogMessage(dirLog, "");
                LogMessage(dirLog, "Current directory contents:");
                PrintDirectoryContents(watchDirectory, dirLog);
                LogMessage(dirLog, "");
                if (notify->NextEntryOffset == 0)
                    break;
                notify = (FILE_NOTIFY_INFORMATION*)((char*)notify + notify->NextEntryOffset);
            } while (1);
        }
        else {
            DWORD error = GetLastError();
            char errorMsg[256];
            sprintf(errorMsg, "Failed to read directory changes: %lu", error);
            LogMessage(dirLog, errorMsg);
            printf("%s\n", errorMsg);
            break;
        }
    }
    LogMessage(dirLog, "Stopping directory monitoring.");
}

void WINAPI ServiceControlHandler(DWORD controlCode) {
    char logMessage[256];
    switch (controlCode) {
    case SERVICE_CONTROL_STOP:
        sprintf(logMessage, "Успех! Сервис %s сменил состояние с %d на %d (Остановлен)", serviceName, serviceStatus.dwCurrentState, SERVICE_STOPPED);
        LogMessage(srvLog, logMessage);
        LogMessage(dirLog, "Directory monitoring stopped.");
        shouldStop = 1;
        UpdateServiceStatus(SERVICE_STOPPED, NO_ERROR);
        if (srvLog) fclose(srvLog);
        if (dirLog) fclose(dirLog);
        break;
    case SERVICE_CONTROL_PAUSE:
        sprintf(logMessage, "Успех! Сервис %s сменил состояние с %d на %d (Приостановлен)", serviceName, serviceStatus.dwCurrentState, SERVICE_PAUSED);
        LogMessage(srvLog, logMessage);
        LogMessage(dirLog, "Directory monitoring paused. Log file closed.");
        fclose(dirLog);
        isPaused = 1;
        UpdateServiceStatus(SERVICE_PAUSED, NO_ERROR);
        break;
    case SERVICE_CONTROL_CONTINUE:
        sprintf(logMessage, "Успех! Сервис %s сменил состояние с %d на %d (Возобновлен)", serviceName, serviceStatus.dwCurrentState, SERVICE_RUNNING);
        LogMessage(srvLog, logMessage);
        isPaused = 0;
        dirLog = fopen(dirLogPath, "a");
        LogMessage(dirLog, "Directory monitoring resumed. Log file reopened.");
        UpdateServiceStatus(SERVICE_RUNNING, NO_ERROR);
        break;
    default:
        break;
    }
}

void WINAPI ServiceMain(DWORD argc, LPSTR* argv) {
    serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, ServiceControlHandler);
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    UpdateServiceStatus(SERVICE_START_PENDING, NO_ERROR);
    if (argc < 3) {
        LogMessage(srvLog, "Попытка запуска провалена! Недостаточно параметров.");
        UpdateServiceStatus(SERVICE_STOPPED, ERROR_INVALID_PARAMETER);
        return;
    }
    strcpy(logDirectory, argv[1]);
    strcpy(watchDirectory, argv[2]);

    char datetime[20];
    GetCurrentDatetime(datetime, sizeof(datetime));

    char srvLogPath[MAX_PATH];
    sprintf(srvLogPath, "%s\\%s-srv.log", logDirectory, datetime);
    srvLog = fopen(srvLogPath, "w");
    if (!srvLog) {
        UpdateServiceStatus(SERVICE_STOPPED, ERROR_OPEN_FAILED);
        return;
    }
    char logMessage[512];
    sprintf(logMessage, "Попытка запуска сервиса %s с параметрами: %s, %s", serviceName, logDirectory, watchDirectory);
    LogMessage(srvLog, logMessage);
    if (_mkdir(logDirectory) == 0) {
        sprintf(logMessage, "Успех! %s создал каталог %s", serviceName, logDirectory);
        LogMessage(srvLog, logMessage);
    }
    else if (errno == EEXIST) {
        sprintf(logMessage, "Успех! %s обнаружил каталог %s", serviceName, logDirectory);
        LogMessage(srvLog, logMessage);
    }
    else {
        sprintf(logMessage, "Попытка запуска провалена! %s не смог создать каталог %s", serviceName, logDirectory);
        LogMessage(srvLog, logMessage);
        UpdateServiceStatus(SERVICE_STOPPED, ERROR_PATH_NOT_FOUND);
        return;
    }
    if (!PathFileExists(watchDirectory)) {
        sprintf(logMessage, "Попытка запуска провалена! %s не смог обнаружить каталог %s", serviceName, watchDirectory);
        LogMessage(srvLog, logMessage);
        UpdateServiceStatus(SERVICE_STOPPED, ERROR_PATH_NOT_FOUND);
        return;
    }

    sprintf(dirLogPath, "%s\\%s-dir.log", logDirectory, datetime);
    dirLog = fopen(dirLogPath, "w");
    if (!dirLog) {
        sprintf(logMessage, "Попытка выполнения операции сервисом провалена! Ошибка открытия %s", dirLogPath);
        LogMessage(srvLog, logMessage);
        fclose(srvLog);
        UpdateServiceStatus(SERVICE_STOPPED, ERROR_OPEN_FAILED);
        return;
    }
    directoryHandle = CreateFile(watchDirectory, FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (directoryHandle == INVALID_HANDLE_VALUE) {
        sprintf(logMessage, "Попытка выполнения операции сервисом провалена! Ошибка открытия каталога %s", watchDirectory);
        LogMessage(srvLog, logMessage);
        fclose(srvLog);
        fclose(dirLog);
        UpdateServiceStatus(SERVICE_STOPPED, GetLastError());
        return;
    }
    sprintf(logMessage, "Успех! Сервис %s запущен с параметрами: %s, %s", serviceName, logDirectory, watchDirectory);
    LogMessage(srvLog, logMessage);
    UpdateServiceStatus(SERVICE_RUNNING, NO_ERROR);

    WatchDirectoryChanges(directoryHandle, dirLog);

    sprintf(logMessage, "Успех! Сервис %s завершает работу.", serviceName);
    LogMessage(srvLog, logMessage);
    fclose(srvLog);
    fclose(dirLog);
    CloseHandle(directoryHandle);
    UpdateServiceStatus(SERVICE_STOPPED, NO_ERROR);
}

int main() {
    CreateNestedDirectories(logDirectory);
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        { "service-09", (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };
    StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}