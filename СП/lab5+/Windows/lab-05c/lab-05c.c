#include <windows.h>
#include <stdio.h>

#define BLOCK_COUNT 10
#define BLOCK_SIZE (256 * 1024) // Размер каждого блока

void HeapInfo(HANDLE heap) {
    PROCESS_HEAP_ENTRY entry;
    entry.lpData = NULL;

    printf("Heap Info:\n");
    printf("Address\t\tSize\t\tFlags\n");

    while (HeapWalk(heap, &entry)) {
        printf("%p\t%zu\t\t%s\n",
            entry.lpData,
            entry.cbData,
            (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) ? "Allocated" : "Free");
    }

    if (GetLastError() != ERROR_NO_MORE_ITEMS) {
        printf("HeapWalk failed. Error: %lu\n", GetLastError());
    }
    printf("\n");
}

int main() {
    HANDLE heap;
    void* blocks[BLOCK_COUNT] = { 0 };

    // Создание кучи
    heap = HeapCreate(0, 1024 * 1024, 8 * 1024 * 1024); // 1 МБ начальный, 8 МБ максимальный
    if (!heap) {
        printf("Heap creation failed. Error: %lu\n", GetLastError());
        return 1;
    }

    printf("Heap created successfully.\n");
    HeapInfo(heap);
    system("pause & cls");

    // Выделение блоков памяти
    for (int i = 0; i < BLOCK_COUNT; i++) {
        blocks[i] = HeapAlloc(heap, HEAP_ZERO_MEMORY, BLOCK_SIZE);
        if (!blocks[i]) {
            printf("Memory allocation failed for block %d. Error: %lu\n", i, GetLastError());
            break;
        }
        printf("Allocated block %d of size %d KB.\n", i + 1, BLOCK_SIZE / 1024);

        HeapInfo(heap);
        system("pause & cls");
    }

    // Освобождение блоков памяти
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (blocks[i]) {
            HeapFree(heap, 0, blocks[i]);
            printf("Freed block %d.\n", i + 1);
        }
    }

    HeapInfo(heap);
    system("pause & cls");

    // Уничтожение кучи
    if (HeapDestroy(heap)) {
        printf("Heap destroyed successfully.\n");
    }
    else {
        printf("Heap destruction failed. Error: %lu\n", GetLastError());
    }

    return 0;
}
