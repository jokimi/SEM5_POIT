#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 64
#define SURNAME_SIZE 128
#define ID_SIZE 8

struct Student {
    char Name[NAME_SIZE];
    char Surname[SURNAME_SIZE];
    unsigned char Course;
    unsigned char Group;
    char ID[ID_SIZE];
};

struct Student* students = NULL;
int listSize = 0;
int fileDescriptor = -1;

void OpenMapping(const char* filePath, int size) {
    if (fileDescriptor != -1) {
        printf("Ошибка: маппинг уже открыт.\n");
        return;
    }

    fileDescriptor = open(filePath, O_RDWR | O_CREAT, 0644);
    if (fileDescriptor == -1) {
        perror("Ошибка при открытии файла");
        return;
    }

    size_t fileSize = sizeof(struct Student) * size;
    if (ftruncate(fileDescriptor, fileSize) == -1) {
        perror("Ошибка при установке размера файла");
        close(fileDescriptor);
        fileDescriptor = -1;
        return;
    }

    students = (struct Student*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (students == MAP_FAILED) {
        perror("Ошибка при маппинге файла");
        close(fileDescriptor);
        fileDescriptor = -1;
        return;
    }

    listSize = size;
    printf("Маппинг успешно открыт. Размер списка: %d\n", listSize);
}

void AddRow(int pos) {
    if (fileDescriptor == -1 || students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("Ошибка: некорректная позиция. Допустимые позиции от 0 до %d.\n", listSize - 1);
        return;
    }

    if (strlen(students[pos].Name) > 0) {
        printf("Ошибка: позиция %d уже занята.\n", pos);
        return;
    }

    struct Student newStudent;
    memset(&newStudent, 0, sizeof(newStudent));
    printf("Введите имя: ");
    fgets(newStudent.Name, sizeof(newStudent.Name), stdin);
    newStudent.Name[strcspn(newStudent.Name, "\n")] = '\0';
    printf("Введите фамилию: ");
    fgets(newStudent.Surname, sizeof(newStudent.Surname), stdin);
    newStudent.Surname[strcspn(newStudent.Surname, "\n")] = '\0';
    printf("Введите курс: ");
    scanf("%hhu", &newStudent.Course);
    printf("Введите группу: ");
    scanf("%hhu", &newStudent.Group);
    printf("Введите ID: ");
    scanf("%s", newStudent.ID);
    getchar();

    students[pos] = newStudent;

    printf("Студент добавлен на позицию %d.\n", pos);
}

void RemRow(int pos) {
    if (fileDescriptor == -1 || students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("Ошибка: некорректная позиция. Допустимые позиции от 0 до %d.\n", listSize - 1);
        return;
    }

    memset(&students[pos], 0, sizeof(struct Student));
    printf("Студент на позиции %d удалён.\n", pos);
}

void PrintRow(int pos) {
    if (fileDescriptor == -1 || students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("Ошибка: некорректная позиция. Допустимые позиции от 0 до %d.\n", listSize - 1);
        return;
    }

    struct Student student = students[pos];
    if (strlen(student.Name) == 0) {
        printf("Позиция %d пуста.\n", pos);
    }
    else {
        printf("Позиция %d:\n", pos);
        printf("\tИмя: %s\n", student.Name);
        printf("\tФамилия: %s\n", student.Surname);
        printf("\tКурс: %d\n", student.Course);
        printf("\tГруппа: %d\n", student.Group);
        printf("\tID: %s\n", student.ID);
    }
}

void PrintRows() {
    if (fileDescriptor == -1 || students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }

    for (int i = 0; i < listSize; i++) {
        if (strlen(students[i].Name) == 0) {
            printf("Позиция %d: Пусто\n", i);
        }
        else {
            printf("Позиция %d:\n", i);
            printf("\tИмя: %s\n", students[i].Name);
            printf("\tФамилия: %s\n", students[i].Surname);
            printf("\tКурс: %d\n", students[i].Course);
            printf("\tГруппа: %d\n", students[i].Group);
            printf("\tID: %s\n", students[i].ID);
        }
    }
}

void CloseMapping() {
    if (fileDescriptor == -1 || students == NULL) {
        printf("Нет маппинга для закрытия.\n");
        return;
    }

    munmap(students, sizeof(struct Student) * listSize);
    close(fileDescriptor);
    students = NULL;
    fileDescriptor = -1;
    printf("Маппинг закрыт.\n");
}