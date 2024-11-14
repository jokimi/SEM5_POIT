#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_MUTEX_NAME_LENGTH 256

HANDLE hMutex;

int is_prime(int num) {
    if (num < 2)
        return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

void find_primes(int lower, int upper) {
    char buffer[BUFFER_SIZE];
    int count = 0;

    for (int i = lower; i <= upper; i++) {
        if (is_prime(i)) {
            count += snprintf(buffer + count, BUFFER_SIZE - count, "%d ", i);
            if (count >= BUFFER_SIZE) {
                fprintf(stderr, "Buffer overflow prevented\n");
                break; // Защита от переполнения буфера
            }
        }
    }

    WaitForSingleObject(hMutex, INFINITE);
    if (count > 0) {
        DWORD bytesWritten;
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, count, &bytesWritten, NULL);
        fflush(stdout); // Очистка буфера
    }
    ReleaseMutex(hMutex);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <lower_bound> <upper_bound>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    char mutex_name[MAX_MUTEX_NAME_LENGTH];
    DWORD result = GetEnvironmentVariableA("MUTEX", mutex_name, MAX_MUTEX_NAME_LENGTH);

    if (result == 0 || result >= MAX_MUTEX_NAME_LENGTH) {
        snprintf(mutex_name, MAX_MUTEX_NAME_LENGTH, "default_mutex");
    }

    hMutex = CreateMutexA(NULL, FALSE, mutex_name);
    if (hMutex == NULL) {
        fprintf(stderr, "CreateMutex error: %lu\n", GetLastError());
        exit(EXIT_FAILURE);
    }

    find_primes(lower, upper);
    CloseHandle(hMutex);
    return EXIT_SUCCESS;
}