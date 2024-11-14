#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <wctype.h>
#include <locale.h>

#define BUFFER_SIZE 256
#define DEFAULT_FIFO "/tmp/lab03_fifo"
#define DEFAULT_SEM "/lab03_sem"

void to_uppercase_wchar(char* str) {
    setlocale(LC_CTYPE, "en_US.UTF-8");  // Set locale for Unicode

    wchar_t wstr[BUFFER_SIZE];
    mbstowcs(wstr, str, BUFFER_SIZE);  // Convert to wide characters

    for (wchar_t* p = wstr; *p; ++p) {
        *p = towupper(*p);  // Transform each character to uppercase
    }

    wcstombs(str, wstr, BUFFER_SIZE);  // Convert back to multibyte
}

int main(int argc, char* argv[]) {
    const char* pipe_name = (argc > 1) ? argv[1] : DEFAULT_FIFO;

    if (mkfifo(pipe_name, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO");
        return EXIT_FAILURE;
    }

    sem_t* sem = sem_open(DEFAULT_SEM, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("Error creating semaphore");
        return EXIT_FAILURE;
    }

    printf("Server is running and waiting for clients...\n");

    while (1) {
        int fd = open(pipe_name, O_RDONLY);
        if (fd == -1) {
            perror("Error opening FIFO for reading");
            continue;
        }

        size_t length;
        if (read(fd, &length, sizeof(length)) != sizeof(length)) {
            perror("Error reading length from FIFO");
            close(fd);
            continue;
        }

        if (length > BUFFER_SIZE) {
            fprintf(stderr, "Error: Buffer overflow\n");
            close(fd);
            continue;
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead = read(fd, buffer, length);
        if (bytesRead < 0) {
            perror("Error reading string from FIFO");
            close(fd);
            continue;
        }
        buffer[bytesRead] = '\0';

        printf("Received string: %s\n", buffer);
        to_uppercase_wchar(buffer);
        printf("Transformed string: %s\n", buffer);

        sleep(5);
        close(fd);

        fd = open(pipe_name, O_WRONLY);
        if (fd == -1) {
            perror("Error opening FIFO for writing");
            continue;
        }

        ssize_t bytesWritten = write(fd, buffer, strlen(buffer) + 1);
        if (bytesWritten < 0) {
            perror("Error writing to FIFO");
        }
        else {
            printf("Sent response: %s\n", buffer);
        }

        close(fd);
    }

    unlink(pipe_name);
    sem_close(sem);
    sem_unlink(DEFAULT_SEM);

    return EXIT_SUCCESS;
}
