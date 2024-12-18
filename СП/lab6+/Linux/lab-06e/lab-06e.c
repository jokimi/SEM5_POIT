#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_ROW_LENGTH 1024
#define MAX_LINES 100

int g_fd = -1;
char* g_buffer = NULL;
size_t g_bufferSize = 0;

void OpenFileCustom(const char* filePath) {
    if (g_fd != -1) {
        printf("Файл уже открыт. Закройте текущий файл перед открытием нового.\n");
        return;
    }
    g_fd = open(filePath, O_RDWR);
    if (g_fd == -1) {
        perror("Не удалось открыть файл");
        return;
    }
    g_bufferSize = MAX_ROW_LENGTH * MAX_LINES;
    g_buffer = (char*)malloc(g_bufferSize);
    if (!g_buffer) {
        perror("Не удалось выделить память для буфера");
        close(g_fd);
        g_fd = -1;
        return;
    }
    printf("Файл успешно открыт: %s\n", filePath);
}

void CloseFile() {
    if (g_fd == -1) {
        printf("Нет открытого файла для закрытия.\n");
        return;
    }
    close(g_fd);
    g_fd = -1;
    if (g_buffer) {
        free(g_buffer);
        g_buffer = NULL;
    }
    g_bufferSize = 0;
    printf("Файл успешно закрыт.\n");
}

void AddRow(const char* row, int pos) {
    if (g_fd == -1) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    if (!row || strlen(row) >= MAX_ROW_LENGTH) {
        fprintf(stderr, "Недопустимая строка.\n");
        return;
    }
    lseek(g_fd, 0, SEEK_SET);
    ssize_t bytesRead = read(g_fd, g_buffer, g_bufferSize - 1);
    if (bytesRead == -1) {
        perror("Ошибка чтения файла");
        return;
    }
    g_buffer[bytesRead] = '\0';

    // Разделить файл на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_r(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_r(NULL, "\n", &context);
    }
    if (pos == 0 || pos == 1) {
        pos = 1;
    }
    else if (pos == -1) {
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

    // Перезаписать файл
    ftruncate(g_fd, 0);
    lseek(g_fd, 0, SEEK_SET);
    for (int i = 0; i < lineCount; ++i) {
        if (write(g_fd, lines[i], strlen(lines[i])) == -1 ||
            write(g_fd, "\n", 1) == -1) {
            perror("Ошибка записи в файл");
            return;
        }
    }
    printf("Строка добавлена.\n");
}

void RemRow(int pos) {
    if (g_fd == -1) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    lseek(g_fd, 0, SEEK_SET);
    ssize_t bytesRead = read(g_fd, g_buffer, g_bufferSize - 1);
    if (bytesRead == -1) {
        perror("Ошибка чтения файла");
        return;
    }
    g_buffer[bytesRead] = '\0';

    // Разделить содержимое файла на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_r(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_r(NULL, "\n", &context);
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
    for (int i = pos - 1; i < lineCount - 1; ++i) {
        lines[i] = lines[i + 1];
    }
    --lineCount;
    if (ftruncate(g_fd, 0) == -1 || lseek(g_fd, 0, SEEK_SET) == -1) {
        perror("Ошибка очистки файла");
        return;
    }
    for (int i = 0; i < lineCount; ++i) {
        if (write(g_fd, lines[i], strlen(lines[i])) == -1 ||
            write(g_fd, "\n", 1) == -1) {
            perror("Ошибка записи в файл");
            return;
        }
    }
    printf("Строка удалена.\n");
}

void PrintRow(int pos) {
    if (g_fd == -1) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    lseek(g_fd, 0, SEEK_SET);
    ssize_t bytesRead = read(g_fd, g_buffer, g_bufferSize - 1);
    if (bytesRead == -1) {
        perror("Ошибка чтения файла");
        return;
    }
    g_buffer[bytesRead] = '\0';

    // Разделить файл на строки
    char* lines[MAX_LINES];
    int lineCount = 0;
    char* context;
    char* line = strtok_r(g_buffer, "\n", &context);
    while (line && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
        line = strtok_r(NULL, "\n", &context);
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
    if (g_fd == -1) {
        fprintf(stderr, "Файл не открыт.\n");
        return;
    }
    lseek(g_fd, 0, SEEK_SET);
    ssize_t bytesRead = read(g_fd, g_buffer, g_bufferSize - 1);
    if (bytesRead == -1) {
        perror("Ошибка чтения файла");
        return;
    }
    g_buffer[bytesRead] = '\0';
    printf("%s\n", g_buffer);
}
int main() {
    char filePath[256];
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
            filePath[strcspn(filePath, "\n")] = 0;
            OpenFileCustom(filePath);
            break;
        case 2:
            printf("Введите строку: ");
            fgets(row, sizeof(row), stdin);
            row[strcspn(row, "\n")] = 0;
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