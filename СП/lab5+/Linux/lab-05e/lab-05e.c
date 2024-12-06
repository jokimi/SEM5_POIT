#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// Определение размера страницы
size_t get_page_size() {
    return sysconf(_SC_PAGESIZE);
}

int main() {
    size_t page_size = get_page_size();
    size_t num_pages = 256; // Резервируем 256 страниц
    size_t region_size = num_pages * page_size;
    size_t half_pages = num_pages / 2;

    printf("Размер страницы: %zu байт\n", page_size);

    // Резервирование виртуальной памяти
    void* reserved_memory = mmap(NULL, region_size, PROT_NONE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (reserved_memory == MAP_FAILED) {
        perror("Ошибка при резервировании памяти");
        exit(EXIT_FAILURE);
    }

    printf("Зарезервированная память: %p - %p\n", reserved_memory,
        (void*)((char*)reserved_memory + region_size));

    // Ожидание для анализа состояния памяти
    printf("Нажмите Enter для продолжения...\n");
    getchar();

    // Выделение физической памяти для второй половины
    void* second_half = (void*)((char*)reserved_memory + half_pages * page_size);
    if (mprotect(second_half, half_pages * page_size, PROT_READ | PROT_WRITE) != 0) {
        perror("Ошибка при выделении физической памяти");
        munmap(reserved_memory, region_size);
        exit(EXIT_FAILURE);
    }

    printf("Выделена физическая память для: %p - %p\n", second_half,
        (void*)((char*)second_half + half_pages * page_size));

    // Ожидание для анализа состояния памяти
    printf("Нажмите Enter для продолжения...\n");
    getchar();

    // Заполнение второй половины последовательностью целых чисел
    int* int_array = (int*)second_half;
    size_t num_ints = (half_pages * page_size) / sizeof(int);
    for (size_t i = 0; i < num_ints; i++) {
        int_array[i] = i;
    }

    printf("Заполнена последовательность целых чисел от 0 до %zu\n", num_ints - 1);

    // Ожидание для анализа состояния памяти
    printf("Нажмите Enter для продолжения...\n");
    getchar();

    // Изменение атрибутов защиты страниц на "только чтение"
    if (mprotect(second_half, half_pages * page_size, PROT_READ) != 0) {
        perror("Ошибка при изменении защиты страниц на только чтение");
        munmap(reserved_memory, region_size);
        exit(EXIT_FAILURE);
    }

    printf("Атрибуты защиты изменены на 'только чтение' для: %p - %p\n", second_half,
        (void*)((char*)second_half + half_pages * page_size));

    // Ожидание для анализа состояния памяти
    printf("Нажмите Enter для продолжения...\n");
    getchar();

    // Освобождение 128 виртуальных страниц (вторая половина)
    if (munmap(second_half, half_pages * page_size) != 0) {
        perror("Ошибка при освобождении виртуальной памяти");
        exit(EXIT_FAILURE);
    }

    printf("Освобождена память: %p - %p\n", second_half,
        (void*)((char*)second_half + half_pages * page_size));

    // Ожидание для анализа состояния памяти
    printf("Нажмите Enter для продолжения...\n");
    getchar();

    // Освобождение оставшихся страниц виртуальной памяти
    if (munmap(reserved_memory, half_pages * page_size) != 0) {
        perror("Ошибка при освобождении оставшейся памяти");
        exit(EXIT_FAILURE);
    }

    printf("Остальная память освобождена: %p - %p\n", reserved_memory,
        (void*)((char*)reserved_memory + half_pages * page_size));

    // Ожидание перед завершением
    printf("Нажмите Enter для завершения программы...\n");
    getchar();

    return 0;
}