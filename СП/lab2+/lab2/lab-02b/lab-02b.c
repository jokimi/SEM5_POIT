#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    LPCWSTR lab02xmodPath = L"..\\lab-02x-mod\\lab-02x-mod.exe";
    LPCWSTR params2 = L"5";

    STARTUPINFO si1, si2, si3;
    PROCESS_INFORMATION pi1, pi2, pi3;

    ZeroMemory(&si1, sizeof(si1));
    ZeroMemory(&si2, sizeof(si2));
    ZeroMemory(&si3, sizeof(si3));
    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);
    si3.cb = sizeof(si3);

    // Создание первого процесса
    BOOL proc1 = CreateProcessW(
        lab02xmodPath,
        NULL,  // Параметры не передаются
        NULL, NULL, FALSE,
        0,
        NULL, NULL,
        &si1, &pi1
    );

    if (proc1) {
        wprintf(L"First process created\n");
    }
    else {
        wprintf(L"First process not created, error: %d\n", GetLastError());
    }

    // Создание второго процесса
    WCHAR cmdLine2[256];
    swprintf(cmdLine2, sizeof(cmdLine2) / sizeof(WCHAR), L"\"%s\" %s", lab02xmodPath, params2);
    BOOL proc2 = CreateProcessW(
        NULL,
        cmdLine2,
        NULL, NULL, FALSE,
        0,
        NULL, NULL,
        &si2, &pi2
    );

    if (proc2) {
        wprintf(L"Second process created\n");
    }
    else {
        wprintf(L"Second process not created, error: %d\n", GetLastError());
    }

    // Установка переменной окружения ITER_NUM
    SetEnvironmentVariableA("ITER_NUM", "6");

    // Создание третьего процесса с передачей параметра
    WCHAR cmdLine3[256];
    swprintf(cmdLine3, sizeof(cmdLine3) / sizeof(WCHAR), L"\"%s\"", lab02xmodPath);
    BOOL proc3 = CreateProcessW(
        NULL,
        cmdLine3,
        NULL, NULL, FALSE,
        0,
        NULL, NULL,
        &si3, &pi3
    );

    if (proc3) {
        wprintf(L"Third process created\n");
    }
    else {
        wprintf(L"Third process not created, error: %d\n", GetLastError());
    }

    // Ожидание завершения процессов
    if (proc1) WaitForSingleObject(pi1.hProcess, INFINITE);
    if (proc2) WaitForSingleObject(pi2.hProcess, INFINITE);
    if (proc3) WaitForSingleObject(pi3.hProcess, INFINITE);

    // Закрытие дескрипторов
    if (proc1) {
        CloseHandle(pi1.hThread);
        CloseHandle(pi1.hProcess);
    }
    if (proc2) {
        CloseHandle(pi2.hThread);
        CloseHandle(pi2.hProcess);
    }
    if (proc3) {
        CloseHandle(pi3.hThread);
        CloseHandle(pi3.hProcess);
    }

    wprintf(L"All processes finished\n");
    system("pause");
    return 0;
}