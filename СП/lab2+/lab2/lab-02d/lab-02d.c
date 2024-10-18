#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL,
        (LPSTR)"..\\lab-02inf\\lab-02inf.exe",
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si, &pi)) {
        printf("Error creating process Lab-02inf: %d\n", GetLastError());
        return 1;
    }

    printf("Lab-02inf started. PID of child process %d\n", GetProcessId(pi.hProcess));
    printf("press any button...\n");
    getchar();

    if (!TerminateProcess(pi.hProcess, 0)) {
        printf("Error terminating child process: %d\n", GetLastError());

    }
    else {
        printf("Child process terminated.\n");
    }

    printf("press any button...\n");
    getchar();


    printf("Lab-02inf stopped. PID of child process %d\n", GetProcessId(pi.hProcess));

    printf("press any button...\n");
    getchar();


    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("descriptor of child process closed\n");
    printf("trying to get PID of child process: %d\n", GetProcessId(pi.hProcess));
    printf("Press any button\n");
    getchar();
    getchar();

    return 0;
}