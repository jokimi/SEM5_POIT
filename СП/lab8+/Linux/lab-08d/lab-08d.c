#include <stdio.h>
#include <dlfcn.h>

typedef void (*OPEN_MAPPING)(const char*, int);
typedef void (*ADD_ROW)(int);
typedef void (*REM_ROW)(int);
typedef void (*PRINT_ROW)(int);
typedef void (*PRINT_ROWS)(void);
typedef void (*CLOSE_MAPPING)(void);

int main() {
    void* handle = dlopen("./libmapping-d.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
        return 1;
    }

    OPEN_MAPPING OpenMapping = (OPEN_MAPPING)dlsym(handle, "OpenMapping");
    ADD_ROW AddRow = (ADD_ROW)dlsym(handle, "AddRow");
    REM_ROW RemRow = (REM_ROW)dlsym(handle, "RemRow");
    PRINT_ROW PrintRow = (PRINT_ROW)dlsym(handle, "PrintRow");
    PRINT_ROWS PrintRows = (PRINT_ROWS)dlsym(handle, "PrintRows");
    CLOSE_MAPPING CloseMapping = (CLOSE_MAPPING)dlsym(handle, "CloseMapping");

    if (!OpenMapping || !AddRow || !RemRow || !PrintRow || !PrintRows || !CloseMapping) {
        fprintf(stderr, "Ошибка поиска функций: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    int choice, pos, size;
    char filePath[260];

    do {
        printf("\nМеню:\n");
        printf("1. Открыть маппинг\n");
        printf("2. Добавить студента\n");
        printf("3. Удалить студента\n");
        printf("4. Вывести студента\n");
        printf("5. Вывести всех студентов\n");
        printf("6. Закрыть маппинг\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        getchar(); // Убираем символ новой строки

        switch (choice) {
        case 1:
            printf("Введите путь к файлу: ");
            fgets(filePath, sizeof(filePath), stdin);
            filePath[strcspn(filePath, "\n")] = '\0';
            printf("Введите размер списка: ");
            scanf("%d", &size);
            getchar();
            OpenMapping(filePath, size);
            break;
        case 2:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            AddRow(pos);
            break;
        case 3:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            RemRow(pos);
            break;
        case 4:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            PrintRow(pos);
            break;
        case 5:
            PrintRows();
            break;
        case 6:
            CloseMapping();
            break;
        case 0:
            CloseMapping();
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (choice != 0);

    dlclose(handle);
    return 0;
}