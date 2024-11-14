#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define BUFFER_SIZE 2048

pthread_mutex_t mutex;

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void find_primes(int lower, int upper, const char* fifo_path) {
    char buffer[BUFFER_SIZE];
    int count = 0;

    for (int i = lower; i <= upper; i++) {
        if (is_prime(i)) {
            count += snprintf(buffer + count, BUFFER_SIZE - count, "%d ", i);
            if (count >= BUFFER_SIZE) break; // Защита от переполнения буфера
        }
    }

    // Ожидание мьютекса и вывод
    pthread_mutex_lock(&mutex);
    if (count > 0) {
        int fifo_fd = open(fifo_path, O_WRONLY);
        if (fifo_fd != -1) {
            printf("Writing to FIFO: %s\n", buffer); // Отладочный вывод
            write(fifo_fd, buffer, count);
            close(fifo_fd);
        }
        else {
            perror("Failed to open FIFO for writing");
        }
    }
    else {
        printf("No prime numbers found in range %d to %d\n", lower, upper); // Отладочный вывод
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <lower_bound> <upper_bound> <mutex_name> <fifo_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    const char* mutex_name = argv[3]; // не используется, но оставлено для совместимости
    const char* fifo_path = argv[4];

    // Инициализация мьютекса
    pthread_mutex_init(&mutex, NULL);

    find_primes(lower, upper, fifo_path);
    pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}