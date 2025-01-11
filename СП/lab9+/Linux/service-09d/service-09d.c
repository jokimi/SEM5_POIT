#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/inotify.h>

#define CONFIG_FILE "/etc/service-09d/service-09d.conf"
#define LOG_DIR "/var/log/service-09d"
#define MAX_BUFFER 1024

FILE* srv_log_file = NULL;
FILE* dir_log_file = NULL;

const char* get_current_datetime() {
    static char buffer[20];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", tm_info);
    return buffer;
}

void create_log_directory() {
    if (access(LOG_DIR, F_OK) == -1) {
        if (mkdir(LOG_DIR, 0755) == -1) {
            perror("Ошибка создания каталога для логов");
            exit(1);
        }
        else {
            printf("[%s] Каталог для логов создан: %s\n", get_current_datetime(), LOG_DIR);
        }
    }
    else {
        printf("[%s] Каталог для логов уже существует: %s\n", get_current_datetime(), LOG_DIR);
    }
}

void write_log(FILE* log, const char* message) {
    if (log == NULL) {
        perror("Ошибка: лог-файл не открыт");
        exit(1);
    }

    fprintf(log, "[%s] %s\n", get_current_datetime(), message);
    fflush(log); // Обеспечиваем запись на диск
}

void write_directory_log(const char* message) {
    write_log(dir_log_file, message);
}

void write_service_log(const char* message) {
    write_log(srv_log_file, message);
}

void log_directory_contents(const char* directory) {
    DIR* dir = opendir(directory);
    if (dir == NULL) {
        perror("Ошибка открытия каталога для вывода содержимого");
        write_directory_log("Ошибка открытия каталога для вывода содержимого.");
        return;
    }

    struct dirent* entry;
    char log_msg[MAX_BUFFER];

    snprintf(log_msg, sizeof(log_msg), "Содержимое каталога '%s':", directory);
    write_directory_log(log_msg);

    while ((entry = readdir(dir)) != NULL) {
        snprintf(log_msg, sizeof(log_msg), "  %s", entry->d_name);
        write_directory_log(log_msg);
    }

    closedir(dir);
    write_directory_log("Конец содержимого каталога.");
}

void watch_directory(const char* directory) {
    int fd, wd;
    char buffer[MAX_BUFFER];

    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        return;
    }

    wd = inotify_add_watch(fd, directory, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE);
    if (wd == -1) {
        perror("inotify_add_watch");
        close(fd);
        return;
    }

    printf("[%s] Начинаем мониторинг каталога: %s\n", get_current_datetime(), directory);
    log_directory_contents(directory);

    while (1) {
        int length;

        // Читаем события inotify с обработкой прерываний
        while ((length = read(fd, buffer, MAX_BUFFER)) < 0) {
            if (errno == EINTR) {
                continue;  // Прерывание из-за сигнала - пробуем снова
            }
            perror("Ошибка чтения из inotify");
            close(fd);
            return;
        }

        int i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                char log_msg[256];

                if (event->mask & IN_CREATE) {
                    snprintf(log_msg, sizeof(log_msg), "Добавление: %s", event->name);
                    write_directory_log(log_msg);
                }
                else if (event->mask & IN_DELETE) {
                    snprintf(log_msg, sizeof(log_msg), "Удаление: %s", event->name);
                    write_directory_log(log_msg);
                }
                else if (event->mask & IN_MODIFY) {
                    snprintf(log_msg, sizeof(log_msg), "Изменение: %s", event->name);
                    write_directory_log(log_msg);
                }
                else if (event->mask & IN_MOVED_TO) {
                    snprintf(log_msg, sizeof(log_msg), "Переименование: %s", event->name);
                    write_directory_log(log_msg);
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }

        log_directory_contents(directory);
    }

    close(fd);
}

void reload_configuration() {
    FILE* config_file = fopen(CONFIG_FILE, "r");
    if (config_file == NULL) {
        write_service_log("Ошибка при открытии конфигурационного файла.");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), config_file) != NULL) {
        write_service_log(line);
    }

    fclose(config_file);
    write_service_log("Конфигурация успешно перезагружена.");
}

void handle_term_signal(int sig) {
    write_service_log("Получен сигнал SIGTERM. Демон завершает свою работу.");
    // Закрываем лог-файлы перед завершением
    if (srv_log_file)
        fclose(srv_log_file);
    if (dir_log_file)
        fclose(dir_log_file);
    exit(0);
}

void handle_hup_signal(int sig) {
    write_service_log("Получен сигнал SIGHUP. Перезагрузка демона.");
    reload_configuration();
}

void setup_signal_handlers() {
    struct sigaction sa;

    sa.sa_handler = handle_term_signal;
    sa.sa_flags = SA_RESTART;  // Перезапуск прерванных вызовов
    sigemptyset(&sa.sa_mask);  // Инициализация пустого набора сигналов
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Ошибка установки обработчика SIGTERM");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_hup_signal;
    sa.sa_flags = SA_RESTART;  // Перезапуск прерванных вызовов
    sigemptyset(&sa.sa_mask);  // Инициализация пустого набора сигналов
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        perror("Ошибка установки обработчика SIGHUP");
        exit(EXIT_FAILURE);
    }
}

void start_daemon(const char* directory) {
    create_log_directory();
    char srv_log_filename[256];
    snprintf(srv_log_filename, sizeof(srv_log_filename), "%s/%s-srv.log", LOG_DIR, get_current_datetime());
    srv_log_file = fopen(srv_log_filename, "a");
    if (srv_log_file == NULL) {
        perror("Ошибка открытия srv.log");
        exit(1);
    }

    char dir_log_filename[256];
    snprintf(dir_log_filename, sizeof(dir_log_filename), "%s/%s-dir.log", LOG_DIR, get_current_datetime());
    dir_log_file = fopen(dir_log_filename, "a");
    if (dir_log_file == NULL) {
        perror("Ошибка открытия dir.log");
        fclose(srv_log_file); // Закрываем ранее открытый файл
        exit(1);
    }

    write_service_log("Попытка запуска демона service-09d.");

    setup_signal_handlers();

    write_service_log("Демон service-09d запущен.");

    DIR* dir = opendir(directory);
    if (dir == NULL) {
        write_service_log("Ошибка! Каталог для отслеживания не существует.");
        perror("Ошибка открытия каталога для отслеживания");
        fclose(srv_log_file);
        fclose(dir_log_file);
        exit(1);
    }
    closedir(dir);

    watch_directory(directory);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <путь к каталогу для отслеживания>\n", argv[0]);
        exit(1);
    }

    const char* directoryPath = argv[1];
    start_daemon(directoryPath);

    return 0;
}