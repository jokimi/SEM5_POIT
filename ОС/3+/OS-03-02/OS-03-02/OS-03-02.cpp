#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

void createChildProcess(const wchar_t* processName) {
    setlocale(LC_ALL, "ru");
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Создание дочернего процесса
    BOOL bRes = CreateProcessW(processName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (!bRes) {
        printf("Ошибка создания процесса: %d\n", GetLastError());
        return;
    }

    // Ожидание завершения дочернего процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрытие дескрипторов процесса и потока
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    DWORD processID = GetCurrentProcessId();
    wcout << L"Идентификатор процесса OS-03-02: " << processID << endl;

    createChildProcess(L"D:\\BSTU\\5 sem\\ОС\\Лабы\\3\\OS-03-02\\OS-03-02-1\\x64\\Debug\\OS-03-02-1.exe");
    createChildProcess(L"D:\\BSTU\\5 sem\\ОС\\Лабы\\3\\OS-03-02\\OS-03-02-2\\x64\\Debug\\OS-03-02-2.exe");

    for (int i = 0; i < 100; ++i) {
        wcout << L"OS-03-02: итерация " << i + 1 << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}