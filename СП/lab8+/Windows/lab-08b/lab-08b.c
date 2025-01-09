#include <stdio.h>
#include <windows.h>

// Определение типов функций для явного импорта
typedef void (*OPEN_MAPPING)(const char*, int);
typedef void (*ADD_ROW)(int);
typedef void (*REM_ROW)(int);
typedef void (*PRINT_ROW)(int);
typedef void (*PRINT_ROWS)(void);
typedef void (*CLOSE_MAPPING)(void);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int choice, pos, size;
    char filePath[260];

    // Дескрипторы библиотек
    HMODULE hMappingd = LoadLibrary("mapping-d.dll");
    HMODULE hMappingd2 = LoadLibrary("mapping-d2.dll");

    if (!hMappingd || !hMappingd2) {
        printf("Ошибка: не удалось загрузить библиотеки.\n");
        return 1;
    }

    OPEN_MAPPING OpenMapping = (OPEN_MAPPING)GetProcAddress(hMappingd, "_OpenMapping");
    ADD_ROW AddRow = (ADD_ROW)GetProcAddress(hMappingd, "_AddRow");
    REM_ROW RemRow = (REM_ROW)GetProcAddress(hMappingd, "_RemRow");
    PRINT_ROW PrintRow = (PRINT_ROW)GetProcAddress(hMappingd, "_PrintRow");
    PRINT_ROWS PrintRows = (PRINT_ROWS)GetProcAddress(hMappingd, "_PrintRows");
    FARPROC CloseMapping2 = GetProcAddress(hMappingd2, (LPCSTR)2);
    if (!CloseMapping2) {
        printf("Ошибка: не удалось получить функцию по порядковому номеру из Mappingd2.dll.\n");
        FreeLibrary(hMappingd);
        FreeLibrary(hMappingd2);
        return 1;
    }
    CLOSE_MAPPING CloseMapping2Func = (CLOSE_MAPPING)CloseMapping2;

    if (!OpenMapping || !AddRow || !RemRow || !PrintRow || !PrintRows || !CloseMapping2) {
        printf("Ошибка: не удалось получить адреса функций из Mappingd.dll.\n");
        FreeLibrary(hMappingd);
        FreeLibrary(hMappingd2);
        return 1;
    }

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
        getchar();

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
            CloseMapping2();
            break;
        case 0:
            CloseMapping2();
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (choice != 0);

    FreeLibrary(hMappingd);
    FreeLibrary(hMappingd2);

    return 0;
}