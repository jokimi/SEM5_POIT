#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 256
#define DEFAULT_FIFO "/tmp/lab03_fifo"
#define DEFAULT_SEM "/lab03_sem"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <string> [pipe_name] [semaphore_name]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* str = argv[1];
    const char* pipe_name = (argc > 2) ? argv[2] : DEFAULT_FIFO;
    const char* sem_name = (argc > 3) ? argv[3] : DEFAULT_SEM;

    sem_t* sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        perror("Error opening semaphore");
        return EXIT_FAILURE;
    }

    sem_wait(sem);

    sleep(7);
    int fd = open(pipe_name, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO for writing");
        sem_post(sem);
        return EXIT_FAILURE;
    }

    sleep(5);

    size_t length = strlen(str) + 1;
    if (write(fd, &length, sizeof(length)) != sizeof(length)) {
        perror("Error writing length to FIFO");
        close(fd);
        sem_post(sem);
        return EXIT_FAILURE;
    }

    if (write(fd, str, length) != length) {
        perror("Error writing string to FIFO");
        close(fd);
        sem_post(sem);
        return EXIT_FAILURE;
    }

    close(fd);

    fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO for reading");
        sem_post(sem);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead < 0) {
        perror("Error reading from FIFO");
    }
    else {
        buffer[bytesRead] = '\0';
        printf("Received response: %s\n", buffer);
    }

    close(fd);
    sem_post(sem);
    sem_close(sem);

    return EXIT_SUCCESS;
}