#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int iterations;

    // Проверка наличия аргумента командной строки
    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    else {
        // Проверка переменной окружения ITER_NUM
        char* envVar = getenv("ITER_NUM");
        if (envVar) {
            iterations = atoi(envVar);
            printf("Using ITER_NUM from environment: %s\n", envVar);
        }
        else {
            fprintf(stderr, "Error: ITER_NUM is not set.\n");
            return EXIT_FAILURE;
        }
    }

    printf("Iterations: %d\n", iterations);
    for (int i = 0; i < iterations; i++) {
        printf("Iteration %d, PID: %d\n", i + 1, getpid());
        usleep(500000); // Пауза на 500 мс
    }

    return EXIT_SUCCESS;
}