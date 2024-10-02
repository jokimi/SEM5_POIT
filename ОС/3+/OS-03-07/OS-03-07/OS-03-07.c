#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char* command = "/mnt/d/BSTU/5 sem/ОС/Лабы/3/OS-03-05/build/OS-03-05/OS-03-05"; // Путь в WSL или Linux
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Дочерний процесс
        system(command);
        char* args[] = { command, NULL };
        execvp(args[0], args); // Используем exec для запуска дочернего процесса
        perror("execvp failed"); // Если execvp вернет управление, произошла ошибка
        exit(1);
    }
    else {
        for (int i = 0; i < 100; i++) {
            printf("OS-03-07 PID: %d, Iteration: %d\n", getpid(), i + 1);
            sleep(1);
        }
        // Ожидание завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}