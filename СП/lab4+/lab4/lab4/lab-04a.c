#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PRIMES 1000000

CRITICAL_SECTION criticalSection;
int* globalPrimes;
int globalPrimeCount = 0;
DWORD tlsIndex;

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

DWORD WINAPI L4Thread(LPVOID lpParam) {
    ThreadParams* params = (ThreadParams*)lpParam;
    int start = params->start;
    int end = params->end;
    // Получаем указатель на локальный буфер из TLS
    int* localPrimes = (int*)TlsGetValue(tlsIndex);
    if (localPrimes == NULL) {
        localPrimes = (int*)malloc(MAX_PRIMES * sizeof(int));
        TlsSetValue(tlsIndex, localPrimes);
    }
    int localPrimeCount = 0;
    for (int i = start; i <= end; ++i) {
        if (IsPrime(i)) {
            localPrimes[localPrimeCount++] = i;
        }
    }
    EnterCriticalSection(&criticalSection);
    for (int i = 0; i < localPrimeCount; ++i) {
        globalPrimes[globalPrimeCount++] = localPrimes[i];
    }
    LeaveCriticalSection(&criticalSection);
    free(localPrimes);
    TlsSetValue(tlsIndex, NULL);
    free(params);
    system("pause");
    return 0;
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
    InitializeCriticalSection(&criticalSection);
    tlsIndex = TlsAlloc();
    if (tlsIndex == TLS_OUT_OF_INDEXES) {
        printf("Failed to allocate TLS index.\n");
        return 1;
    }
    globalPrimes = (int*)malloc(MAX_PRIMES * sizeof(int));
    if (globalPrimes == NULL) {
        printf("Failed to allocate memory for global primes.\n");
        return 1;
    }
    HANDLE* threads = (HANDLE*)malloc(threadCount * sizeof(HANDLE));
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
        threads[i] = CreateThread(NULL, 0, L4Thread, params, 0, NULL);
        if (threads[i] == NULL) {
            printf("Error creating thread.\n");
            return 1;
        }
    }
    WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
    for (int i = 0; i < threadCount; ++i) {
        CloseHandle(threads[i]);
    }
    free(threads);
    // Сортировка и вывод результатов
    qsort(globalPrimes, globalPrimeCount, sizeof(int), Compare);
    printf("Found %d prime numbers:\n", globalPrimeCount);
    for (int i = 0; i < globalPrimeCount; ++i)
        printf("%d ", globalPrimes[i]);
    printf("\n");
    DeleteCriticalSection(&criticalSection);
    TlsFree(tlsIndex);
    free(globalPrimes);
    return 0;
}