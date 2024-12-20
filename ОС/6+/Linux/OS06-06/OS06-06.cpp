﻿#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#define DELAY_MS 100

pthread_mutex_t mutex;

pthread_t create_thread(void* (*function)(void*), void* args) {
    pthread_t thread;
    pthread_create(&thread, NULL, function, args);
    return thread;
}

void* loop(void* name) {
    char* thread_name = (char*)name; // Приводим void* к char*
    int pid = getpid();
    struct timespec timespec;
    timespec.tv_sec = DELAY_MS / 1000;
    timespec.tv_nsec = (DELAY_MS % 1000) * 1000000;

    for (int i = 1; i <= 90; i++) {
        if (i == 30)
            pthread_mutex_lock(&mutex);
        else if (i == 60)
            pthread_mutex_unlock(&mutex);

        printf("  [%s]\t  %d.  PID = %d\n", thread_name, i, pid);
        nanosleep(&timespec, &timespec);
    }

    printf("\n=======  %s finished  =======\n\n", thread_name);
    pthread_exit((void*)"done"); // Приводим строку к void*
}

int main(int argc, char* argv[]) {
    int pid = getpid();
    int size = 2;
    const char* names[] = { "A", "B" }; // Используем const char*
    pthread_t threads[size];

    struct timespec timespec;
    timespec.tv_sec = DELAY_MS / 1000;
    timespec.tv_nsec = (DELAY_MS % 1000) * 1000000;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < size; i++)
        threads[i] = create_thread(loop, (void*)names[i]);

    for (int i = 1; i <= 90; i++) {
        if (i == 30)
            pthread_mutex_lock(&mutex);
        else if (i == 60)
            pthread_mutex_unlock(&mutex);

        printf("[MAIN]\t  %d.  PID = %d\n", i, pid);
        nanosleep(&timespec, &timespec);
    }

    printf("\n=====  MAIN finished  =====\n\n");

    for (int i = 0; i < size; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}