#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void PrintInfo(const char* fileName) {
    struct stat fileStat;
    if (stat(fileName, &fileStat) == -1) {
        perror("Невозможно получить информацию о файле");
        return;
    }
    printf("Имя файла: %s\n", fileName);
    double sizeBytes = (double)fileStat.st_size;
    printf("Размер: %.0f Б, %.2f КиБ, %.2f МиБ\n",
        sizeBytes, sizeBytes / 1024.0, sizeBytes / (1024.0 * 1024.0));
    const char* fileType = strrchr(fileName, '.');
    printf("Тип файла: %s\n", fileType ? fileType : "Unknown");
    char timeBuffer[26];
    struct tm* timeInfo;
    timeInfo = localtime(&fileStat.st_mtime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("Время последнего изменения: %s\n", timeBuffer);
    timeInfo = localtime(&fileStat.st_atime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("Время последнего доступа: %s\n", timeBuffer);
#ifdef __linux__
    timeInfo = localtime(&fileStat.st_ctime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("Время создания: %s\n", timeBuffer);
#else
    printf("Время создания: Недоступно на данной платформе\n");
#endif
}

void PrintText(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Невохможно открыть файл");
        return;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (!isprint(c) && !isspace(c)) {
            fprintf(stderr, "Файл не является текстовым\n");
            fclose(file);
            return;
        }
        putchar(c);
    }
    putchar('\n');
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <путь_к_файлу>\n", argv[0]);
        return 1;
    }
    const char* filePath = argv[1];
    PrintInfo(filePath);
    PrintText(filePath);
    return 0;
}