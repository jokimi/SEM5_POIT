#include <stdio.h>
#include <locale.h>

// Глобальные переменные
int global_var_init = 10;           // Инициализированная глобальная переменная
int global_var_uninit;              // Неинициализированная глобальная переменная

// Глобальные статические переменные
static int static_global_init = 20; // Инициализированная статическая глобальная переменная
static int static_global_uninit;    // Неинициализированная статическая глобальная переменная

// Функция
void my_function() {
    printf("Функция my_function вызвана.\n");
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    // Локальные переменные
    int local_var_init = 30;        // Инициализированная локальная переменная
    int local_var_uninit;           // Неинициализированная локальная переменная

    // Локальные статические переменные
    static int static_local_init = 40;    // Инициализированная статическая локальная переменная
    static int static_local_uninit;       // Неинициализированная статическая локальная переменная

    // Вывод адресов переменных и функций
    printf("Адрес глобальной инициализированной переменной: %p\n", (void*)&global_var_init);
    printf("Адрес глобальной неинициализированной переменной: %p\n", (void*)&global_var_uninit);
    printf("Адрес глобальной статической инициализированной переменной: %p\n", (void*)&static_global_init);
    printf("Адрес глобальной статической неинициализированной переменной: %p\n", (void*)&static_global_uninit);
    printf("Адрес локальной инициализированной переменной: %p\n", (void*)&local_var_init);
    printf("Адрес локальной неинициализированной переменной: %p\n", (void*)&local_var_uninit);
    printf("Адрес локальной статической инициализированной переменной: %p\n", (void*)&static_local_init);
    printf("Адрес локальной статической неинициализированной переменной: %p\n", (void*)&static_local_uninit);
    printf("Адрес функции my_function: %p\n", (void*)&my_function);
    printf("Адрес argc: %p\n", (void*)&argc);
    printf("Адрес argv: %p\n", (void*)&argv);

    printf("Нажмите Enter для завершения...\n");
    getchar();

    return 0;
}