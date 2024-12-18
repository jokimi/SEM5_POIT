#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 64
#define SURNAME_SIZE 128
#define ID_SIZE 8
#define UNICODE
#define _UNICODE

struct Student {
    char Name[NAME_SIZE];
    char Surname[SURNAME_SIZE];
    unsigned char Course;
    unsigned char Group;
    char ID[ID_SIZE];
};

HANDLE hFileMapping = NULL;
struct Student* students = NULL;
int listSize = 0;

void OpenMapping(const char* filePath, int size) {
    if (hFileMapping != NULL) {
        printf("Ошибка: маппинг уже открыт.\n");
        return;
    }

    HANDLE hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка: не удалось открыть файл.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        printf("Создаём новый файл с размером %d байт.\n", sizeof(struct Student) * size);
        fileSize = sizeof(struct Student) * size;
        SetFilePointer(hFile, fileSize, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
    }

    hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    CloseHandle(hFile);

    if (hFileMapping == NULL) {
        printf("Ошибка при создании маппинга.\n");
        return;
    }

    students = (struct Student*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (students == NULL) {
        printf("Ошибка при отображении файла.\n");
        CloseHandle(hFileMapping);
        hFileMapping = NULL;
        return;
    }

    // Инициализация файла нулями, если это новый файл
    for (int i = 0; i < size; i++) {
        memset(&students[i], 0, sizeof(struct Student));
    }
    FlushViewOfFile(students, sizeof(struct Student) * size);

    listSize = size;
    printf("Маппинг успешно открыт. Размер списка: %d\n", listSize);
}

void AddRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
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
    memset(&newStudent, 0, sizeof(newStudent)); // Очистка памяти для новой записи

    printf("Введите имя: ");
    fgets(newStudent.Name, sizeof(newStudent.Name), stdin);
    newStudent.Name[strcspn(newStudent.Name, "\n")] = '\0';

    printf("Введите фамилию: ");
    fgets(newStudent.Surname, sizeof(newStudent.Surname), stdin);
    newStudent.Surname[strcspn(newStudent.Surname, "\n")] = '\0';

    printf("Введите курс: ");
    if (scanf("%hhu", &newStudent.Course) != 1) {
        printf("Ошибка: некорректный ввод курса.\n");
        getchar();
        return;
    }

    printf("Введите группу: ");
    if (scanf("%hhu", &newStudent.Group) != 1) {
        printf("Ошибка: некорректный ввод группы.\n");
        getchar();
        return;
    }

    printf("Введите ID: ");
    if (scanf("%s", newStudent.ID) != 1) {
        printf("Ошибка: некорректный ввод ID.\n");
        getchar();
        return;
    }
    getchar();

    // Добавление записи
    students[pos] = newStudent;

    // Синхронизация изменений с файлом
    if (!FlushViewOfFile(students, sizeof(struct Student) * listSize)) {
        printf("Ошибка: не удалось сохранить изменения в файл.\n");
    }
    else {
        printf("Студент добавлен на позицию %d.\n", pos);
    }
}

void RemRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
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
        printf("Ошибка: позиция %d пуста.\n", pos);
        return;
    }

    // Очистка записи
    memset(&students[pos], 0, sizeof(struct Student));
    FlushViewOfFile(students, sizeof(struct Student) * listSize);
    printf("Студент на позиции %d удалён.\n", pos);
}

void PrintRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
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
    if (hFileMapping == NULL || students == NULL) {
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
    if (hFileMapping == NULL || students == NULL) {
        printf("Нет маппинга для закрытия.\n");
        return;
    }

    UnmapViewOfFile(students);
    CloseHandle(hFileMapping);
    students = NULL;
    hFileMapping = NULL;
    printf("Маппинг закрыт.\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

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