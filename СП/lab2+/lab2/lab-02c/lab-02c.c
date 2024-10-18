#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void createProcess(const char* exeName) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    char cmdLine[MAX_PATH];
    snprintf(cmdLine, sizeof(cmdLine), "%s", exeName);

    if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed for %s: %lu\n", exeName, GetLastError());
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const char* currentDir = "D:\\BSTU\\5 sem\\—œ\\À‡·˚\\lab2\\current";
    SetCurrentDirectory(currentDir);

    const char* exeNames[] = {
        "lab-02hw-1.exe",
        "lab-02hw-2.exe",
        "lab-02hw-3.exe",
        "lab-02hw-4.exe",
        "lab-02hw-5.exe"
    };

    for (int i = 0; i < 5; i++) {
        createProcess(exeNames[i]);
    }

    return 0;
}