#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

// Глобальные переменные
int global_var_init = 10;
int global_var_uninit;

// Глобальные статические переменные
static int static_global_init = 20;
static int static_global_uninit;

// Функция
void my_function() {
    wprintf(L"Функция my_function вызвана.\n");
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    // Локальные переменные
    int local_var_init = 30;
    int local_var_uninit;

    // Локальные статические переменные
    static int static_local_init = 40;
    static int static_local_uninit;

    wprintf(L"Адрес глобальной инициализированной переменной: %p\n", (void*)&global_var_init);
    wprintf(L"Адрес глобальной неинициализированной переменной: %p\n", (void*)&global_var_uninit);
    wprintf(L"Адрес глобальной статической инициализированной переменной: %p\n", (void*)&static_global_init);
    wprintf(L"Адрес глобальной статической неинициализированной переменной: %p\n", (void*)&static_global_uninit);
    wprintf(L"Адрес локальной инициализированной переменной: %p\n", (void*)&local_var_init);
    wprintf(L"Адрес локальной неинициализированной переменной: %p\n", (void*)&local_var_uninit);
    wprintf(L"Адрес локальной статической инициализированной переменной: %p\n", (void*)&static_local_init);
    wprintf(L"Адрес локальной статической неинициализированной переменной: %p\n", (void*)&static_local_uninit);
    wprintf(L"Адрес функции my_function: %p\n", (void*)&my_function);
    wprintf(L"Адрес argc: %p\n", (void*)&argc);
    wprintf(L"Адрес argv: %p\n", (void*)&argv);

    wprintf(L"Нажмите любую клавишу для завершения...\n");
    getchar();

    return 0;
}