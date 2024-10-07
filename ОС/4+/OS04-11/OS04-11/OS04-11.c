#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Функция потока
void* OS04_11_T1(void* arg) {
    for (int i = 0; i < 75; i++) {
        printf("Thread: PID = %d, Iteration = %d\n", getpid(), i + 1);
        sleep(1); // Задержка в 1 секунду
    }
    return NULL;
}

int main() {
    pthread_t thread;

    // Создание потока
    if (pthread_create(&thread, NULL, OS04_11_T1, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Основной процесс
    for (int i = 0; i < 100; i++) {
        printf("Main: PID = %d, Iteration = %d\n", getpid(), i + 1);
        sleep(1); // Задержка в 1 секунду
    }

    // Ожидание завершения потока
    pthread_join(thread, NULL);

    return 0;
}