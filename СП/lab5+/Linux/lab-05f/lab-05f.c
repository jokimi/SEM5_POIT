#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BLOCK_COUNT 10
#define BLOCK_SIZE (512 * 1024) // Размер каждого блока в байтах (512 КиБ)

int main() {
    void* blocks[BLOCK_COUNT] = { 0 }; // Массив указателей для хранения блоков памяти

    printf("Этап 1: Начинаем выделение памяти из кучи...\n");
    for (int i = 0; i < BLOCK_COUNT; i++) {
        blocks[i] = malloc(BLOCK_SIZE); // Выделяем память
        if (!blocks[i]) {
            fprintf(stderr, "Ошибка: не удалось выделить память для блока %d.\n", i);
            return EXIT_FAILURE;
        }
        printf("Блок %d выделен по адресу: %p\n", i, blocks[i]);
        printf("Нажмите Enter для продолжения...\n");
        getchar();
    }

    printf("\nЭтап 2: Заполнение памяти массивами целых чисел...\n");
    for (int i = 0; i < BLOCK_COUNT; i++) {
        int* int_block = (int*)blocks[i];
        size_t int_count = BLOCK_SIZE / sizeof(int); // Количество целых чисел в блоке
        for (size_t j = 0; j < int_count; j++) {
            int_block[j] = (int)j; // Заполняем массив числами
        }
        printf("Блок %d заполнен целыми числами.\n", i);
        printf("Нажмите Enter для продолжения...\n");
        getchar();
    }

    printf("\nЭтап 3: Освобождение памяти...\n");
    for (int i = 0; i < BLOCK_COUNT; i++) {
        free(blocks[i]);
        printf("Блок %d освобождён.\n", i);
        printf("Нажмите Enter для продолжения...\n");
        getchar();
    }

    printf("\nЭтап 4: Завершение работы программы. Все блоки освобождены.\n");
    printf("Нажмите Enter для выхода...\n");
    getchar();

    return EXIT_SUCCESS;
}