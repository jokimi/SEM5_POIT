#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Начало выполнения программы OS05_05.\n");
    printf("Идентификатор процесса: %d\n", getpid());

    int i = 1;
    while (i <= 1000) {
        usleep(100000); // 100 мс
        i++;
    }

    return 0; // Эта строка никогда не будет достигнута
}