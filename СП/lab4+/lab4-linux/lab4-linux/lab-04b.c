#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PRIMES 1000000

int* global_primes;
int global_count = 0;
pthread_mutex_t mutex;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

__thread int local_primes[MAX_PRIMES];
__thread int local_count = 0;

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void init_mutex() {
    pthread_mutex_init(&mutex, NULL);
}

void* L4Thread(void* arg) {
    int* range = (int*)arg;
    int start = range[0];
    int end = range[1];
    for (int i = start; i <= end; i++) {
        if (is_prime(i))
            if (local_count < MAX_PRIMES) 
                local_primes[local_count++] = i;
    }
    pthread_once(&once_control, init_mutex);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < local_count; i++)
        global_primes[global_count++] = local_primes[i];
    pthread_mutex_unlock(&mutex);
    free(range);
    return NULL;
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
        printf("Invalid arguments.\n");
        return 1;
    }
    global_primes = (int*)malloc(MAX_PRIMES * sizeof(int));
    pthread_t threads[num_threads];
    int range_size = (upper_bound - lower_bound + 1) / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int* range = (int*)malloc(2 * sizeof(int));
        range[0] = lower_bound + i * range_size;
        range[1] = (i == num_threads - 1) ? upper_bound : (range[0] + range_size - 1);
        pthread_create(&threads[i], NULL, L4Thread, range);
    }
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    qsort(global_primes, global_count, sizeof(int), compare);
    printf("Found prime numbers:\n");
    for (int i = 0; i < global_count; i++)
        printf("%d ", global_primes[i]);
    printf("\n");
    pthread_mutex_destroy(&mutex);
    free(global_primes);
    return 0;
}