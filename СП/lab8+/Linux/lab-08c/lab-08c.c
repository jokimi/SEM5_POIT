#include <stdio.h>
#include "mapping-s.h"

int main() {
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
            CloseMapping();
            break;
        case 0:
            CloseMapping();
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (choice != 0);

    return 0;
}