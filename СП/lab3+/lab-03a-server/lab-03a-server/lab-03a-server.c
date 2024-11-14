#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_MUTEX_NAME_LENGTH 256

void create_child_processes(int count, int lower, int upper) {
    HANDLE hMutex;
    HANDLE hPipeRead, hPipeWrite;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    char mutex_name[MAX_MUTEX_NAME_LENGTH];
    DWORD result = GetEnvironmentVariableA("MUTEX", mutex_name, MAX_MUTEX_NAME_LENGTH);

    if (result == 0 || result >= MAX_MUTEX_NAME_LENGTH) {
        snprintf(mutex_name, MAX_MUTEX_NAME_LENGTH, "default_mutex");
    }

    int range_per_process = (upper - lower + 1) / count;
    printf("Creating %d processes with ranges:\n", count);

    for (int i = 0; i < count; i++) {
        hMutex = CreateMutexA(NULL, FALSE, mutex_name);
        if (hMutex == NULL) {
            fprintf(stderr, "CreateMutex error: %lu\n", GetLastError());
            exit(EXIT_FAILURE);
        }

        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
            fprintf(stderr, "CreatePipe error: %lu\n", GetLastError());
            CloseHandle(hMutex);
            exit(EXIT_FAILURE);
        }

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdOutput = hPipeWrite;
        si.dwFlags |= STARTF_USESTDHANDLES;

        int child_lower = lower + i * range_per_process;
        int child_upper = (i == count - 1) ? upper : (lower + (i + 1) * range_per_process - 1);
        printf("Process %d will handle range: %d to %d\n", i + 1, child_lower, child_upper);

        WCHAR cmdLine[256];
        swprintf(cmdLine, sizeof(cmdLine) / sizeof(WCHAR),
            L"\"D:\\BSTU\\5 sem\\СП\\Лабы\\lab3\\lab-03a-client\\build\\lab-03a-client\\lab-03a-client.exe\" %d %d %s",
            child_lower, child_upper, mutex_name);

        if (!CreateProcessW(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "CreateProcess error: %lu for command: %ls\n", GetLastError(), cmdLine);
            CloseHandle(hPipeRead);
            CloseHandle(hPipeWrite);
            CloseHandle(hMutex);
            exit(EXIT_FAILURE);
        }

        CloseHandle(si.hStdOutput);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        CloseHandle(hPipeWrite);

        char buffer[BUFFER_SIZE];
        DWORD bytesRead;
        while (ReadFile(hPipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Received from child: %s\n", buffer);
        }
        Sleep(5000);
        CloseHandle(hPipeRead);
        CloseHandle(hMutex);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <process_count> <lower_bound> <upper_bound>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int process_count = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);

    create_child_processes(process_count, lower_bound, upper_bound);
    Sleep(5000);
    return EXIT_SUCCESS;
}