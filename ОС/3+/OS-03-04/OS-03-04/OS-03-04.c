#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
#ifdef _WIN32
    DWORD pid = GetCurrentProcessId();
    printf("OS-03-04 ID: %lu\n", pid);
#else
    pid_t pid = getpid();
    printf("OS-03-04 ID: %d\n", pid);
#endif

    for (int i = 0; i < 100; i++) {
        printf("Iteration %d\n", i + 1);
#ifdef _WIN32
        Sleep(1000); // Задержка в миллисекундах для Windows
#else
        sleep(1);    // Задержка в секундах для Linux
#endif
    }

    return 0;
}