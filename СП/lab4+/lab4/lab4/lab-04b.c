#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define MAX_PRIMES 1000000

int* global_primes;
int global_count = 0;

__declspec(thread) int local_primes[MAX_PRIMES];
__declspec(thread) int local_count = 0;
CRITICAL_SECTION cs;

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

DWORD WINAPI L4Thread(LPVOID lpParam) {
    int* range = (int*)lpParam;
    int start = range[0];
    int end = range[1];
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            if (local_count < MAX_PRIMES) {
                local_primes[local_count++] = i;
            }
        }
    }
    EnterCriticalSection(&cs);
    for (int i = 0; i < local_count; i++)
        global_primes[global_count++] = local_primes[i];
    LeaveCriticalSection(&cs);
    system("pause");
    return 0;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: lab-04b <threads> <range_start> <range_end>\n");
        return 1;
    }
    int num_threads = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);
    if (num_threads <= 0 || lower_bound < 0 || upper_bound < lower_bound) {
        printf("Неверные аргументы.\n");
        return 1;
    }
    InitializeCriticalSection(&cs);
    // Выделяем память для глобального массива простых чисел
    global_primes = (int*)malloc(MAX_PRIMES * sizeof(int));
    if (global_primes == NULL) {
        printf("Ошибка выделения памяти.\n");
        DeleteCriticalSection(&cs);
        return 1;
    }
    HANDLE* threads = (HANDLE*)malloc(num_threads * sizeof(HANDLE));
    int** ranges = (int**)malloc(num_threads * sizeof(int*));
    int range_size = (upper_bound - lower_bound + 1) / num_threads;
    for (int i = 0; i < num_threads; i++) {
        ranges[i] = (int*)malloc(2 * sizeof(int));
        ranges[i][0] = lower_bound + i * range_size;
        ranges[i][1] = (i == num_threads - 1) ? upper_bound : (ranges[i][0] + range_size - 1);
        threads[i] = CreateThread(NULL, 0, L4Thread, (LPVOID)ranges[i], 0, NULL);
        if (threads[i] == NULL) {
            printf("Ошибка создания потока.\n");
            return 1;
        }
    }
    WaitForMultipleObjects(num_threads, threads, TRUE, INFINITE);
    // Сортировка глобального массива простых чисел
    qsort(global_primes, global_count, sizeof(int), compare);
    printf("Найденные простые числа:\n");
    for (int i = 0; i < global_count; i++)
        printf("%d ", global_primes[i]);
    printf("\n");
    for (int i = 0; i < num_threads; i++) {
        CloseHandle(threads[i]);
        free(ranges[i]);
    }
    free(threads);
    free(ranges);
    free(global_primes);
    DeleteCriticalSection(&cs);
    return 0;
}