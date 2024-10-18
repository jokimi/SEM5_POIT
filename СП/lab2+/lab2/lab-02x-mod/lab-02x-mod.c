#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int iterations; // Глобальная переменная для количества итераций

int main(int argc, char* argv[]) {
    // Проверка наличия аргумента командной строки
    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    else {
        char envVar[256];
        DWORD result = GetEnvironmentVariableA("ITER_NUM", envVar, sizeof(envVar));

        if (result > 0 && result < sizeof(envVar)) {
            iterations = atoi(envVar);
            printf("Using ITER_NUM from environment: %s\n", envVar);
        }
        else {
            fprintf(stderr, "Error: ITER_NUM is not set or too long.\n");
            ExitProcess(1);
        }
    }

    printf("Iterations: %d\n", iterations);
    for (int i = 0; i < iterations; i++) {
        printf("Iteration %d, PID: %d\n", i + 1, GetCurrentProcessId());
        Sleep(500);
    }

    return 0;
}