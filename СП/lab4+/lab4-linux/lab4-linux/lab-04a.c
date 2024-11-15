#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PRIMES 1000000

// Глобальные переменные
pthread_mutex_t mutex;
int* globalPrimes;
int globalPrimeCount = 0;
pthread_key_t tlsKey;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

typedef struct {
    int start;
    int end;
} ThreadParams;

bool IsPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void init_once() {
    pthread_mutex_init(&mutex, NULL);
    pthread_key_create(&tlsKey, free);  // ключ TLS
}

void* L4Thread(void* arg) {
    pthread_once(&once_control, init_once);
    ThreadParams* params = (ThreadParams*)arg;
    int start = params->start;
    int end = params->end;

    // Получаем указатель на локальный буфер из TLS
    int* localPrimes = (int*)pthread_getspecific(tlsKey);
    if (localPrimes == NULL) {
        localPrimes = (int*)malloc(MAX_PRIMES * sizeof(int));
        pthread_setspecific(tlsKey, localPrimes);
    }
    int localPrimeCount = 0;
    for (int i = start; i <= end; ++i) {
        if (IsPrime(i)) {
            localPrimes[localPrimeCount++] = i;
        }
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < localPrimeCount; ++i) {
        globalPrimes[globalPrimeCount++] = localPrimes[i];
    }
    sleep(10);
    pthread_mutex_unlock(&mutex);

    free(params);
    return NULL;
}

int Compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: lab-04a <threads> <range_start> <range_end>\n");
        return 1;
    }

    int threadCount = atoi(argv[1]);
    int rangeStart = atoi(argv[2]);
    int rangeEnd = atoi(argv[3]);

    if (threadCount <= 0 || rangeStart >= rangeEnd) {
        printf("Invalid arguments.\n");
        return 1;
    }

    globalPrimes = (int*)malloc(MAX_PRIMES * sizeof(int));
    if (globalPrimes == NULL) {
        printf("Failed to allocate memory for global primes.\n");
        return 1;
    }

    pthread_t* threads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    if (threads == NULL) {
        printf("Failed to allocate memory for threads.\n");
        return 1;
    }

    int rangeSize = (rangeEnd - rangeStart + 1) / threadCount;
    for (int i = 0; i < threadCount; ++i) {
        int start = rangeStart + i * rangeSize;
        int end = (i == threadCount - 1) ? rangeEnd : start + rangeSize - 1;

        ThreadParams* params = (ThreadParams*)malloc(sizeof(ThreadParams));
        if (params == NULL) {
            printf("Failed to allocate memory for thread parameters.\n");
            return 1;
        }

        params->start = start;
        params->end = end;
        if (pthread_create(&threads[i], NULL, L4Thread, params) != 0) {
            printf("Error creating thread.\n");
            return 1;
        }
    }

    for (int i = 0; i < threadCount; ++i) {
        pthread_join(threads[i], NULL);
    }

    qsort(globalPrimes, globalPrimeCount, sizeof(int), Compare);
    printf("Found %d prime numbers:\n", globalPrimeCount);
    for (int i = 0; i < globalPrimeCount; ++i)
        printf("%d ", globalPrimes[i]);
    printf("\n");

    pthread_key_delete(tlsKey);
    pthread_mutex_destroy(&mutex);
    free(globalPrimes);
    free(threads);

    return 0;
}