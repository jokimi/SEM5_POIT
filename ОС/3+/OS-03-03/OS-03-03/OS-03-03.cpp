#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

void listProcesses() {
    // Получаем дескриптор для создания снимка процессов
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cerr << "Ошибка создания снимка процессов: " << GetLastError() << endl;
        return;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    // Получаем первый процесс
    if (Process32First(hSnapshot, &pe)) {
        do {
            wcout << L"Процесс: " << pe.szExeFile << L" (PID: " << pe.th32ProcessID << L")" << endl;
        } while (Process32Next(hSnapshot, &pe));
    }
    else {
        cerr << "Ошибка получения информации о процессе: " << GetLastError() << endl;
    }

    // Закрываем дескриптор
    CloseHandle(hSnapshot);
}

int main() {
    setlocale(LC_ALL, "ru");
    wcout << L"Список выполняющихся процессов:" << endl;
    listProcesses();
    return 0;
}