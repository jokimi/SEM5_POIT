#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#define FILE_PATH "/mnt/d/BSTU/5 sem/ОС/Лабы/9/Linux/OS09-07.txt"
#define BUFFER_SIZE 256

// Функция для подсчета смещения в байтах для N символов UTF-8
off_t calculate_utf8_offset(const char* filepath, int num_chars) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int char_count = 0;
    off_t offset = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            // Определяем начало UTF-8 символа
            if ((buffer[i] & 0xC0) != 0x80) {
                char_count++;
                if (char_count == num_chars + 1) {
                    offset += i;
                    close(fd);
                    return offset;
                }
            }
        }
        offset += bytes_read;
    }

    close(fd);
    return offset; // Возвращаем полный размер, если не нашли N символов
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    int fd = open(FILE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    // Находим байтовое смещение для 10-го символа
    off_t offset = calculate_utf8_offset(FILE_PATH, 10);
    lseek(fd, offset, SEEK_SET);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, 20); // Читаем до 20 байт (максимум 10 UTF-8 символов)
    if (bytes_read == -1) {
        perror("Ошибка чтения файла");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0';

    printf("Символы 11-20 файла: ");
    for (ssize_t i = 0; i < bytes_read; i++) {
        // Выводим только полные UTF-8 символы
        if ((buffer[i] & 0xC0) != 0x80) { // Если это начало UTF-8 символа
            putchar(buffer[i]);
        }
        else {
            putchar(buffer[i]); // Остальные байты символа
        }
    }
    printf("\n");

    close(fd);
    return 0;
}
