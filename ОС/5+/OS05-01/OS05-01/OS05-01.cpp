#include <iostream>
#include <windows.h>
#include <bitset>

void printProcessAndThreadInfo() {
    // Получаем идентификатор текущего процесса
    DWORD processId = GetCurrentProcessId();
    std::cout << "Process ID: " << processId << std::endl;

    // Получаем идентификатор текущего потока
    DWORD threadId = GetCurrentThreadId();
    std::cout << "Thread ID: " << threadId << std::endl;

    // Получаем дескриптор текущего процесса
    HANDLE hProcess = GetCurrentProcess();

    // Получаем приоритетный класс текущего процесса
    DWORD priorityClass = GetPriorityClass(hProcess);
    std::cout << "Process Priority Class: " << priorityClass << std::endl;

    // Получаем приоритет текущего потока
    HANDLE hThread = GetCurrentThread();
    int threadPriority = GetThreadPriority(hThread);
    std::cout << "Thread Priority: " << threadPriority << std::endl;

    // Получаем маску доступных процессоров (affinity mask)
    DWORD_PTR processAffinityMask, systemAffinityMask;
    if (GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask)) {
        std::cout << "Affinity Mask (binary): " << std::bitset<32>(processAffinityMask) << std::endl;
    }
    else {
        std::cerr << "Failed to get process affinity mask." << std::endl;
    }

    // Получаем количество доступных процессоров
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;

    // Получаем процессор, назначенный текущему потоку
    DWORD_PTR threadAffinityMask;
    if (GetProcessAffinityMask(hProcess, &threadAffinityMask, &systemAffinityMask)) {
        std::cout << "Thread Affinity Mask (binary): " << std::bitset<32>(threadAffinityMask) << std::endl;
    }
    else {
        std::cerr << "Failed to get thread affinity mask." << std::endl;
    }

    // Закрываем дескрипторы
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

int main() {
    printProcessAndThreadInfo();
    return 0;
}