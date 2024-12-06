#include <windows.h>
#include <stdio.h>

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    // Размер страницы

    DWORD pageSize = sysInfo.dwPageSize;
    DWORD totalPages = 256;
    DWORD totalMemory = pageSize * totalPages;

    printf("Page size: %lu bytes\n", pageSize);
    printf("Total memory to reserve: %lu bytes\n", totalMemory);

    // Этап 1. Резервирование виртуальной памяти

    LPVOID reservedMemory = VirtualAlloc(NULL, totalMemory, MEM_RESERVE, PAGE_NOACCESS);
    if (!reservedMemory) {
        printf("Failed to reserve memory. Error: %lu\n", GetLastError());
        return 1;
    }
    printf("Step 1: Reserved memory at address: %p", reservedMemory);
    getchar();

    // Этап 2. Выделение физической памяти для второй половины

    LPVOID committedMemory = VirtualAlloc((BYTE*)reservedMemory + (totalMemory / 2), totalMemory / 2, MEM_COMMIT, PAGE_READWRITE);
    if (!committedMemory) {
        printf("Failed to commit memory. Error: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("Step 2: Committed memory at address: %p", committedMemory);
    getchar();

    // Этап 3. Заполнение памяти числами

    int* data = (int*)committedMemory;
    for (DWORD i = 0; i < (totalMemory / 2) / sizeof(int); i++) {
        data[i] = i;
    }
    printf("Step 3: Filled memory with integers starting from 0.");
    getchar();

    // Этап 4. Изменение атрибутов защиты страниц памяти на "только для чтения"

    DWORD oldProtect;
    if (!VirtualProtect(committedMemory, totalMemory / 2, PAGE_READONLY, &oldProtect)) {
        printf("Failed to change memory protection. Error: %lu\n", GetLastError());
    }
    else {
        printf("Step 4: Memory protection changed to READONLY.");
    }
    getchar();

    // Этап 5. Освобождение физической памяти для 128 страниц

    if (!VirtualFree(committedMemory, totalMemory / 2, MEM_DECOMMIT)) {
        printf("Failed to decommit memory. Error: %lu\n", GetLastError());
    }
    else {
        printf("Step 5: Decommitted memory.");
    }
    getchar();

    // Этап 6. Освобождение виртуальной памяти

    if (!VirtualFree(reservedMemory, 0, MEM_RELEASE)) {
        printf("Failed to release memory. Error: %lu\n", GetLastError());
    }
    else {
        printf("Step 6: Released memory.");
    }
    getchar();

    return 0;
}