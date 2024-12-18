#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <locale.h>

#define MAX_ROW_LENGTH 1024
#define MAX_LINES 100

HANDLE g_hFile = INVALID_HANDLE_VALUE;
char* g_buffer = NULL;
size_t g_bufferSize = 0;

void OpenFileCustom(LPSTR filePath) {
    if (g_hFile != INVALID_HANDLE_VALUE) {
        printf("Файл уже открыт. Закройте текущий файл перед открытием нового.\n");
        return;
    }
    g_hFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Не удалось открыть файл: %s\n", filePath);
        return;
    }

    // Инициализация буфера
    g_bufferSize = MAX_ROW_LENGTH * MAX_LINES; // Максимум строк
    g_buffer = (char*)malloc(g_bufferSize);
    if (!g_buffer) {
        fprintf(stderr, "Не удалось выделить память для буфера.\n");
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
        return;
    }
    printf("Файл успешно открыт: %s\n", filePath);
}

void CloseFile() {
    if (g_hFile == INVALID_HANDLE_VALUE) {
        printf("Нет открытого файла для закрытия.\n");
        return;
    }
    CloseHandle(g_hFile);
    g_hFile = INVALID_HANDLE_VALUE;
    if (g_buffer) {
        free(g_buffer);
        g_buffer = NULL;
    }
    g_bufferSize = 0;
    printf("Файл успешно закрыт.\n");
}

void AddRow(LPSTR row, INT pos) {
    if (g_hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    if (!row || strlen(row) >= MAX_ROW_LENGTH) {
        fprintf(stderr, "Недопустимая строка.\n");
        return;
    }

    // Прочитать файл в буфер
    DWORD bytesRead;
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN);
    if (!ReadFile(g_hFile, g_buffer, g_bufferSize - 1, &bytesRead, NULL)) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        return;
    }
    g_buffer[bytesRead] = '\0';

    // Разделить файл на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_s(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_s(NULL, "\n", &context);
    }
    if (pos == 0 || pos == 1) {
        pos = 1;
    }
    if (pos == -1) {
        pos = lineCount + 1;
    }
    if (pos >= 0 && pos <= lineCount + 1) {
        // Сдвигаем строки на одну позицию вперед
        for (int i = lineCount; i >= pos; --i) {
            lines[i] = lines[i - 1];
        }
        lines[pos - 1] = row;
        ++lineCount;
    }
    else {
        fprintf(stderr, "Неверная позиция.\n");
        return;
    }

    // Перезаписываем файл с новыми данными
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN);
    SetEndOfFile(g_hFile); // Очистить файл перед записью

    for (int i = 0; i < lineCount; ++i) {
        DWORD bytesWritten;
        if (!WriteFile(g_hFile, lines[i], strlen(lines[i]), &bytesWritten, NULL) ||
            !WriteFile(g_hFile, "\n", 1, &bytesWritten, NULL)) {
            fprintf(stderr, "Ошибка записи в файл.\n");
            return;
        }
    }
    printf("Строка добавлена.\n");
}


void RemRow(INT pos) {
    if (g_hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }

    // Прочитать файл в буфер
    DWORD bytesRead;
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN); // Переместить указатель в начало файла
    if (!ReadFile(g_hFile, g_buffer, g_bufferSize - 1, &bytesRead, NULL)) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        return;
    }
    g_buffer[bytesRead] = '\0'; // Завершаем строку

    // Разделить файл на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_s(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_s(NULL, "\n", &context);
    }

    if (pos == 0) {
        pos = 1;
    }
    else if (pos == -1) {
        pos = lineCount;
    }

    if (pos < 1 || pos > lineCount) {
        fprintf(stderr, "Неверная позиция. Допустимые позиции: от 1 до %d.\n", lineCount);
        return;
    }

    // Сдвигаем строки на одну позицию назад
    for (int i = pos - 1; i < lineCount - 1; ++i) {
        lines[i] = lines[i + 1];
    }
    --lineCount;

    // Перезаписываем файл с обновлённым содержимым
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN); // Указатель в начало файла
    SetEndOfFile(g_hFile);                        // Очистить содержимое файла

    for (int i = 0; i < lineCount; ++i) {
        DWORD bytesWritten;
        if (!WriteFile(g_hFile, lines[i], strlen(lines[i]), &bytesWritten, NULL) ||
            !WriteFile(g_hFile, "\n", 1, &bytesWritten, NULL)) {
            fprintf(stderr, "Ошибка записи в файл.\n");
            return;
        }
    }
    printf("Строка на позиции %d удалена.\n", pos);
}

void PrintRow(INT pos) {
    if (g_hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    DWORD bytesRead;
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN); // Переместить указатель в начало файла
    if (!ReadFile(g_hFile, g_buffer, g_bufferSize - 1, &bytesRead, NULL)) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        return;
    }
    g_buffer[bytesRead] = '\0';

    // Разделить файл на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_s(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_s(NULL, "\n", &context);
    }
    if (pos == 0) {
        pos = 1;
    }
    else if (pos == -1) {
        pos = lineCount;
    }
    if (pos < 1 || pos > lineCount) {
        fprintf(stderr, "Неверная позиция. Допустимые позиции: от 1 до %d.\n", lineCount);
        return;
    }
    printf("%s\n", lines[pos - 1]);
}

void PrintRows() {
    if (g_hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }

    // Прочитать файл в буфер
    DWORD bytesRead;
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN);
    if (!ReadFile(g_hFile, g_buffer, g_bufferSize - 1, &bytesRead, NULL)) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        return;
    }
    g_buffer[bytesRead] = '\0';

    printf("%s\n", g_buffer);
}

int main() {
    SetConsoleOutputCP(65001);
    char filePath[MAX_PATH];
    char row[MAX_ROW_LENGTH];
    int choice, pos;
    while (1) {
        printf("\nВыберите выполняемую операцию:\n");
        printf("1. Открыть файл\n");
        printf("2. Вставить строку\n");
        printf("3. Удалить строку\n");
        printf("4. Вывести строку\n");
        printf("5. Вывести файл\n");
        printf("6. Закрыть файл\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                printf("Введите путь к файлу: ");
                fgets(filePath, sizeof(filePath), stdin);
                filePath[strcspn(filePath, "\n")] = 0; // Убираем символ новой строки
                OpenFileCustom(filePath);
                break;
            case 2:
                printf("Введите строку: ");
                fgets(row, sizeof(row), stdin);
                row[strcspn(row, "\n")] = 0; // Убираем символ новой строки
                printf("Введите позицию (0 - начальная, -1 - в конец): ");
                scanf("%d", &pos);
                AddRow(row, pos);
                break;
            case 3:
                printf("Введите позицию строки для удаления: ");
                scanf("%d", &pos);
                RemRow(pos);
                break;
            case 4:
                printf("Введите позицию строки для вывода: ");
                scanf("%d", &pos);
                PrintRow(pos);
                break;
            case 5:
                PrintRows();
                break;
            case 6:
                CloseFile();
                break;
            case 0:
                CloseFile();
                printf("Выход из программы.\n");
                return 0;
            default:
                printf("Неверный выбор.\n");
        }
    }
    return 0;
}