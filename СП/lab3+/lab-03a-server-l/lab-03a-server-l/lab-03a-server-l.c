#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048

void create_child_processes(int count, int lower, int upper, const char* fifo_path) {
    for (int i = 0; i < count; i++) {
        int range_per_process = (upper - lower + 1) / count;
        int child_lower = lower + i * range_per_process;
        int child_upper = (i == count - 1) ? upper : (lower + (i + 1) * range_per_process - 1);

        printf("Creating process for range: %d to %d\n", child_lower, child_upper);

        pid_t pid = fork();
        if (pid == 0) { // Дочерний процесс
            char lower_str[10], upper_str[10];
            snprintf(lower_str, sizeof(lower_str), "%d", child_lower);
            snprintf(upper_str, sizeof(upper_str), "%d", child_upper);

            execl("/mnt/d/bstu/5 sem/сп/лабы/lab3/lab-03a-client-l/build/lab-03a-client-l/lab-03a-client-l", "lab-03a-client-l", lower_str, upper_str, "my_mutex", fifo_path, NULL);
            perror("execl failed");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }


        // Чтение из FIFO
        char buffer[BUFFER_SIZE];
        int fifo_fd = open(fifo_path, O_RDONLY);
        if (fifo_fd != -1) {
            while (read(fifo_fd, buffer, sizeof(buffer) - 1) > 0) {
                buffer[BUFFER_SIZE - 1] = '\0'; // Завершение строки
                printf("Received from child: %s\n", buffer);
            }
            close(fifo_fd);
        }
        else {
            perror("Failed to open FIFO");
        }

        // Ожидание завершения всех дочерних процессов
        while (wait(NULL) > 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <process_count> <lower_bound> <upper_bound>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int process_count = atoi(argv[1]);
    int lower_bound = atoi(argv[2]);
    int upper_bound = atoi(argv[3]);
    const char* fifo_path = "/tmp/fifo_pipe"; // Используйте временный путь

    // Удаление FIFO, если он уже существует
    unlink(fifo_path);

    // Создание FIFO
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("Failed to create FIFO");
        return EXIT_FAILURE;
    }

    create_child_processes(process_count, lower_bound, upper_bound, fifo_path);

    // Удаление FIFO после завершения работы
    unlink(fifo_path);
    return EXIT_SUCCESS;
}