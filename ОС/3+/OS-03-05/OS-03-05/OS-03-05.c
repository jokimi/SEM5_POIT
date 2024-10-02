#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h> // Для _getpid()
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

void child_process() {
    for (int i = 0; i < 50; i++) {
        printf("OS-03-05-1 PID: %d, Iteration: %d\n", getpid(), i + 1);
#ifdef _WIN32
        Sleep(1000); // Задержка в миллисекундах для Windows
#else
        sleep(1);    // Задержка в секундах для Linux
#endif
    }
}

int main() {
#ifdef _WIN32
    // Для Windows используем _spawnlp или _beginthread для создания процесса
    int pid = _spawnlp(_P_WAIT, "OS-03-05-1", "OS-03-05-1", NULL);
#else
    pid_t pid = fork(); // Используем pid_t для Linux
#endif

    if (pid < 0) {
        // Ошибка при создании процесса
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Дочерний процесс
        child_process();
        exit(0);
    }
    else {
        // Родительский процесс
        for (int i = 0; i < 100; i++) {
            printf("OS-03-05 PID: %d, Iteration: %d\n", getpid(), i + 1);
#ifdef _WIN32
            Sleep(1000); // Задержка в миллисекундах для Windows
#else
            sleep(1);    // Задержка в секундах для Linux
#endif
        }
        // Ожидание завершения дочернего процесса
#ifdef _WIN32
        // В Windows нет wait, так что просто выходим
#else
        wait(NULL);
#endif
    }

    return 0;
}