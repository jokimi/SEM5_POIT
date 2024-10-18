#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    // Создание первого дочернего процесса
    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork first child");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Первый дочерний процесс
        char* args[] = { "/mnt/d/bstu/5 sem/сп/лабы/lab2-linux/build/lab-02x-linux/lab-02x-linux", "10", NULL }; // Передаем 10 итераций
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Создание второго дочернего процесса
    pid2 = fork();
    if (pid2 < 0) {
        perror("Failed to fork second child");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Второй дочерний процесс
        setenv("ITER_NUM", "20", 1); // Установка переменной окружения
        char* args[] = { "/mnt/d/bstu/5 sem/сп/лабы/lab2-linux/build/lab-02x-linux/lab-02x-linux", NULL }; // Не передаем аргументы
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    // Родительский процесс ждет завершения дочерних процессов
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Both child processes have completed.\n");
    return EXIT_SUCCESS;
}