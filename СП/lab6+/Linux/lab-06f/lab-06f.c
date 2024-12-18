#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <unistd.h>

#define MAX_BUFFER 1024

void PrintDirectoryContents(const char* sDir) {
    DIR* dir;
    struct dirent* entry;
    dir = opendir(sDir);
    if (dir == NULL) {
        perror("Ошибка при открытии каталога");
        return;
    }
    printf("Содержимое каталога %s:\n", sDir);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (entry->d_type == DT_DIR) {
                printf("[Каталог] %s\n", entry->d_name);
            }
            else {
                printf("[Файл] %s\n", entry->d_name);
            }
        }
    }
    closedir(dir);
}

void WatchDirectory(const char* sDir) {
    int fd, wd;
    char buffer[MAX_BUFFER];
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        return;
    }
    wd = inotify_add_watch(fd, sDir, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE);
    if (wd == -1) {
        perror("inotify_add_watch");
        return;
    }
    printf("Начинаем мониторинг каталога: %s\n", sDir);
    printf("Ожидание изменений в каталоге...\n");
    while (1) {
        int length = read(fd, buffer, MAX_BUFFER);
        if (length < 0) {
            perror("read");
            break;
        }
        int i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("Добавление: %s\n", event->name);
                }
                else if (event->mask & IN_DELETE) {
                    printf("Удаление: %s\n", event->name);
                }
                else if (event->mask & IN_MODIFY) {
                    printf("Изменение: %s\n", event->name);
                }
                else if (event->mask & IN_MOVED_TO) {
                    printf("Переименование: %s\n", event->name);
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8");
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <путь к каталогу>\n", argv[0]);
        return 1;
    }
    const char* directoryPath = argv[1];
    DIR* dir = opendir(directoryPath);
    if (dir == NULL) {
        perror("Каталог не существует");
        return 1;
    }
    closedir(dir);
    PrintDirectoryContents(directoryPath);
    WatchDirectory(directoryPath);
    return 0;
}