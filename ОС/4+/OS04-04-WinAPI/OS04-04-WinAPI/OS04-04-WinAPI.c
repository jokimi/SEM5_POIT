#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadZed(LPVOID lpParam) {
    for (int i = 0; i < 10; i++) {
        printf(" (Z-%d) ", GetCurrentThreadId());
        Sleep(1000);
    }
    printf(" Поток Z завершается\n");
    return 0;
}

DWORD WINAPI ThreadWithParam(LPVOID lpParam) {
    for (int i = 0; i < 20; i++) {
        printf(" (%s-%d) ", (char*)lpParam, GetCurrentThreadId());
        Sleep(1000);
    }
    printf(" Поток %s завершается\n", (char*)lpParam);
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);
    HANDLE t1, t1a, t1b;

    // Создание потоков
    t1 = CreateThread(NULL, 0, ThreadZed, NULL, 0, NULL);
    t1a = CreateThread(NULL, 0, ThreadWithParam, "Елизавета", 0, NULL);
    t1b = CreateThread(NULL, 0, ThreadWithParam, "Козека", 0, NULL);

    // Главный поток работает 5 секунд
    for (int i = 0; i < 5; i++) {
        printf(" (*-%d) ", GetCurrentThreadId());
        Sleep(1000);
    }
    printf("Главный поток завершается\n");

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t1a, INFINITE);
    WaitForSingleObject(t1b, INFINITE);

    CloseHandle(t1);
    CloseHandle(t1a);
    CloseHandle(t1b);

    return 0;
}