#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

void DetectBinaryType(LPSTR FileName) {
    FILE* file = fopen(FileName, "rb");
    if (!file) {
        fprintf(stderr, "Невозможно открыть файл для определения бинарного типа: %s\n", FileName);
        return;
    }
    unsigned char buffer[8];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);
    if (bytesRead < 4) {
        printf("Тип бинарного файла: Unknown (файл слишком маленький)\n");
        return;
    }

    // Проверка сигнатур файлов
    if (memcmp(buffer, "\x89PNG", 4) == 0) {
        printf("Тип бинарного файла: PNG Image\n");
    }
    else if (memcmp(buffer, "BM", 2) == 0) {
        printf("Тип бинарного файла: BMP Image\n");
    }
    else if (memcmp(buffer, "GIF", 3) == 0) {
        printf("Тип бинарного файла: GIF Image\n");
    }
    else if (memcmp(buffer, "\xFF\xD8\xFF", 3) == 0) {
        printf("Тип бинарного файла: JPEG Image\n");
    }
    else if (memcmp(buffer, "MZ", 2) == 0) {
        printf("Тип бинарного файла: Windows Executable (EXE)\n");
    }
    else if (memcmp(buffer, "\x25\x50\x44\x46", 4) == 0) {
        printf("Тип бинарного файла: PDF Document\n");
    }
    else if (memcmp(buffer, "PK\x03\x04", 4) == 0) {
        printf("Тип бинарного файла: DOCX Document or ZIP Archive\n");
    }
    else {
        printf("Тип бинарного файла: Unknown\n");
    }
}

void PrintInfo(LPSTR FileName) {
    WIN32_FIND_DATA fileData;
    HANDLE hFile = FindFirstFile(FileName, &fileData);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Файл %s не найден\n", FileName);
        return;
    }
    printf("Имя файла: %s\n", fileData.cFileName);
    LARGE_INTEGER fileSize;
    fileSize.LowPart = fileData.nFileSizeLow;
    fileSize.HighPart = fileData.nFileSizeHigh;
    double sizeBytes = (double)fileSize.QuadPart;
    printf("Размер файла: %.0f Б, %.2f КиБ, %.2f МиБ\n",
        sizeBytes, sizeBytes / 1024.0, sizeBytes / (1024.0 * 1024.0));
    HANDLE fileHandle = CreateFile(
        FileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (fileHandle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Ошибка открытия файла для определения типа.\n");
    }
    else {
        DWORD fileType = GetFileType(fileHandle);
        switch (fileType) {
        case FILE_TYPE_DISK:
            printf("Тип файла: Дисковый файл\n");
            break;
        case FILE_TYPE_CHAR:
            printf("Тип файла: Символьное устройство\n");
            break;
        case FILE_TYPE_PIPE:
            printf("Тип файла: Канал (pipe)\n");
            break;
        case FILE_TYPE_UNKNOWN:
        default:
            printf("Тип файла: Неизвестный\n");
            break;
        }
        CloseHandle(fileHandle);
    }

    // Время создания/доступа/изменения
    SYSTEMTIME creationTime, lastAccessTime, lastWriteTime;
    FileTimeToSystemTime(&fileData.ftCreationTime, &creationTime);
    FileTimeToSystemTime(&fileData.ftLastAccessTime, &lastAccessTime);
    FileTimeToSystemTime(&fileData.ftLastWriteTime, &lastWriteTime);

    printf("Время создания: %04d-%02d-%02d\n",
        creationTime.wYear, creationTime.wMonth, creationTime.wDay);
    printf("Время последнего доступа: %04d-%02d-%02d\n",
        lastAccessTime.wYear, lastAccessTime.wMonth, lastAccessTime.wDay);
    printf("Время последнего изменения: %04d-%02d-%02d\n",
        lastWriteTime.wYear, lastWriteTime.wMonth, lastWriteTime.wDay);

    // Определяем, является ли файл бинарным
    bool isBinary = false;
    FILE* file = fopen(FileName, "rb");
    if (file) {
        int c;
        while ((c = fgetc(file)) != EOF) {
            if (c == '\0') {
                isBinary = true;
                break;
            }
        }
        fclose(file);
    }
    printf("Бинарный файл: %s\n", isBinary ? "Да" : "Нет");
    if (isBinary)
        DetectBinaryType(FileName);
    FindClose(hFile);
}

void PrintText(LPSTR FileName) {
    FILE* file = fopen(FileName, "r");
    if (!file) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", FileName);
        return;
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (!isprint(c) && !isspace(c)) {
            fprintf(stderr, "Файл не является текстовым.\n");
            fclose(file);
            return;
        }
        putchar(c);
    }
    putchar('\n');
    fclose(file);
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    if (argc < 2) {
        fprintf(stderr, "Использование: lab-06a <путь_к_файлу>\n");
        return 1;
    }
    LPSTR filePath = argv[1];
    PrintInfo(filePath);
    PrintText(filePath);
    return 0;
}