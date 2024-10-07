#include <iostream>
#include <windows.h>

int main() {
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    std::cout << "Process ID: " << processId << std::endl;
    std::cout << "Thread ID: " << threadId << std::endl;

    for (int i = 0; i < 100; ++i) {
        Sleep(1000);
        std::cout << "Iteration: " << (i + 1) << std::endl;
    }

    return 0;
}