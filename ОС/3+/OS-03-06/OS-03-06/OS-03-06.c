#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char* command = "./OS-03-06-1"; // Путь к дочернему процессу
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Дочерний процесс
        execlp(command, command, NULL);
        perror("execlp failed"); // Если execlp вернет управление, произошла ошибка
        exit(1);
    }
    else {
        // Родительский процесс
        for (int i = 0; i < 100; i++) {
            printf("OS-03-06 PID: %d, Iteration: %d\n", getpid(), i + 1);
            sleep(1);
        }
        // Ожидание завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}