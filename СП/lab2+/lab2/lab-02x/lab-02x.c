#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    int iterations;

    // Проверка наличия аргумента командной строки
    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    else {
        fprintf(stderr, "Error: No arguments provided.\n");
        ExitProcess(1); // Завершение работы с ошибкой
    }

    printf("Iterations: %d\n", iterations);
    for (int i = 0; i < iterations; i++) {
        printf("Iteration %d, PID: %d\n", i + 1, GetCurrentProcessId());
        Sleep(500); // Задержка 500 мс
    }

    return 0;
}