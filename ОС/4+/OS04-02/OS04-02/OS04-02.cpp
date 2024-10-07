#include <iostream>
#include <windows.h>

DWORD WINAPI OS04_02_T1(LPVOID lpParam);
DWORD WINAPI OS04_02_T2(LPVOID lpParam);

int main() {
    // Получаем идентификатор текущего процесса
    DWORD processId = GetCurrentProcessId();
    std::cout << "Process ID: " << processId << std::endl;

    // Создание потоков
    HANDLE thread1 = CreateThread(NULL, 0, OS04_02_T1, NULL, 0, NULL);
    if (thread1 == NULL) {
        std::cerr << "Failed to create Thread 1. Error: " << GetLastError() << std::endl;
        return 1; // Завершаем программу при ошибке
    }

    HANDLE thread2 = CreateThread(NULL, 0, OS04_02_T2, NULL, 0, NULL);
    if (thread2 == NULL) {
        std::cerr << "Failed to create Thread 2. Error: " << GetLastError() << std::endl;
        CloseHandle(thread1); // Закрываем дескриптор первого потока
        return 1; // Завершаем программу при ошибке
    }

    // Основной поток выполняет цикл 100 итераций
    for (int i = 0; i < 100; ++i) {
        Sleep(1000); // Задержка 1 секунда
        std::cout << "Main Thread Iteration: " << (i + 1) << std::endl;
    }

    // Ожидание завершения потоков
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    // Закрытие дескрипторов потоков
    CloseHandle(thread1);
    CloseHandle(thread2);

    return 0;
}

DWORD WINAPI OS04_02_T1(LPVOID lpParam) {
    DWORD threadId = GetCurrentThreadId();
    DWORD processId = GetCurrentProcessId();

    for (int i = 0; i < 50; ++i) {
        Sleep(1000); // Задержка 1 секунда
        std::cout << "Thread 1 - Process ID: " << processId << ", Thread ID: " << threadId << ", Iteration: " << (i + 1) << std::endl;
    }
    return 0;
}

DWORD WINAPI OS04_02_T2(LPVOID lpParam) {
    DWORD threadId = GetCurrentThreadId();
    DWORD processId = GetCurrentProcessId();

    for (int i = 0; i < 125; ++i) {
        Sleep(1000); // Задержка 1 секунда
        std::cout << "Thread 2 - Process ID: " << processId << ", Thread ID: " << threadId << ", Iteration: " << (i + 1) << std::endl;
    }
    return 0;
}