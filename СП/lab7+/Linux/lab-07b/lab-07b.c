#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

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

int fileDescriptor = -1;
struct Student* students = NULL;
int listSize = 0;

void OpenMapping(const char* filePath, int size) {
    if (students != NULL) {
        printf("Ошибка: маппинг уже открыт.\n");
        return;
    }

    fileDescriptor = open(filePath, O_RDWR | O_CREAT, 0644);
    if (fileDescriptor == -1) {
        perror("Ошибка при открытии файла");
        return;
    }

    // Определяем размер файла и при необходимости расширяем его
    off_t fileSize = lseek(fileDescriptor, 0, SEEK_END);
    if (fileSize == 0) {
        printf("Создаём новый файл с размером %d байт.\n", sizeof(struct Student) * size);
        fileSize = sizeof(struct Student) * size;
        if (ftruncate(fileDescriptor, fileSize) == -1) {
            perror("Ошибка при установке размера файла");
            close(fileDescriptor);
            fileDescriptor = -1;
            return;
        }
    }

    students = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    if (students == MAP_FAILED) {
        perror("Ошибка при отображении файла");
        close(fileDescriptor);
        fileDescriptor = -1;
        students = NULL;
        return;
    }

    // Инициализация файла нулями, если это новый файл
    for (int i = 0; i < size; i++) {
        memset(&students[i], 0, sizeof(struct Student));
    }

    listSize = size;
    printf("Маппинг успешно открыт. Размер списка: %d\n", listSize);
}

void AddRow(int pos) {
    if (students == NULL) {
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

    // Ввод данных студента
    struct Student newStudent;
    memset(&newStudent, 0, sizeof(newStudent));

    printf("Введите имя: ");
    fgets(newStudent.Name, sizeof(newStudent.Name), stdin);
    newStudent.Name[strcspn(newStudent.Name, "\n")] = '\0';

    printf("Введите фамилию: ");
    fgets(newStudent.Surname, sizeof(newStudent.Surname), stdin);
    newStudent.Surname[strcspn(newStudent.Surname, "\n")] = '\0';

    printf("Введите курс: ");
    if (scanf("%hhu", &newStudent.Course) != 1) {
        printf("Ошибка ввода курса.\n");
        getchar();
        return;
    }

    printf("Введите группу: ");
    if (scanf("%hhu", &newStudent.Group) != 1) {
        printf("Ошибка ввода группы.\n");
        getchar();
        return;
    }

    printf("Введите ID: ");
    if (scanf("%s", newStudent.ID) != 1) {
        printf("Ошибка ввода ID.\n");
        getchar();
        return;
    }
    getchar();

    // Добавление студента в массив
    students[pos] = newStudent;

    // Синхронизация с файлом
    msync(students, sizeof(struct Student) * listSize, MS_SYNC);
    printf("Студент добавлен на позицию %d.\n", pos);
}

void RemRow(int pos) {
    if (students == NULL) {
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
    if (strlen(students[pos].Name) == 0) {
        printf("Ошибка: позиция %d уже пуста.\n", pos);
        return;
    }

    // Очистка записи студента
    memset(&students[pos], 0, sizeof(struct Student));

    // Синхронизация с файлом
    msync(students, sizeof(struct Student) * listSize, MS_SYNC);
    printf("Студент на позиции %d удалён.\n", pos);
}

void PrintRow(int pos) {
    if (students == NULL) {
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
    if (students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }

    for (int i = 0; i < listSize; i++) {
        printf("Позиция %d: ", i);
        if (strlen(students[i].Name) == 0) {
            printf("Пусто\n");
        }
        else {
            printf("\n");
            printf("\tИмя: %s\n", students[i].Name);
            printf("\tФамилия: %s\n", students[i].Surname);
            printf("\tКурс: %d\n", students[i].Course);
            printf("\tГруппа: %d\n", students[i].Group);
            printf("\tID: %s\n", students[i].ID);
        }
    }
}

void CloseMapping() {
    if (students == NULL) {
        printf("Ошибка: маппинг не открыт.\n");
        return;
    }

    munmap(students, sizeof(struct Student) * listSize);
    close(fileDescriptor);
    students = NULL;
    fileDescriptor = -1;
    printf("Маппинг закрыт.\n");
}

int main() {
    int choice, pos, size;
    char filePath[260];

    do {
        printf("\nМеню:\n");
        printf("1. Открыть маппинг\n");
        printf("2. Добавить студента\n");
        printf("3. Удалить студента\n");
        printf("4. Вывести студента\n");
        printf("5. Вывести всех студентов\n");
        printf("6. Закрыть маппинг\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        getchar(); // Убираем символ новой строки

        switch (choice) {
        case 1:
            printf("Введите путь к файлу: ");
            fgets(filePath, sizeof(filePath), stdin);
            filePath[strcspn(filePath, "\n")] = '\0';
            printf("Введите размер списка: ");
            scanf("%d", &size);
            getchar();
            OpenMapping(filePath, size);
            break;
        case 2:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            AddRow(pos);
            break;
        case 3:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            RemRow(pos);
            break;
        case 4:
            printf("Введите позицию: ");
            scanf("%d", &pos);
            getchar();
            PrintRow(pos);
            break;
        case 5:
            PrintRows();
            break;
        case 6:
            CloseMapping();
            break;
        case 0:
            CloseMapping();
            break;
        default:
            printf("Некорректный выбор.\n");
        }
    } while (choice != 0);

    return 0;
}