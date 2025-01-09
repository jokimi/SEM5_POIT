#include <windows.h>
#include <stdio.h>
#include <string.h>

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

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

HANDLE hFileMapping = NULL;
struct Student* students = NULL;
int listSize = 0;

__declspec(dllexport) void OpenMapping(const char* filePath, int size) {
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

__declspec(dllexport) void AddRow(int pos) {
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
    students[pos] = newStudent;
    if (!FlushViewOfFile(students, sizeof(struct Student) * listSize)) {
        printf("Ошибка: не удалось сохранить изменения в файл.\n");
    }
    else {
        printf("Студент добавлен на позицию %d.\n", pos);
    }
}

__declspec(dllexport) void RemRow(int pos) {
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
    memset(&students[pos], 0, sizeof(struct Student));
    FlushViewOfFile(students, sizeof(struct Student) * listSize);
    printf("Студент на позиции %d удалён.\n", pos);
}

__declspec(dllexport) void PrintRow(int pos) {
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

__declspec(dllexport) void PrintRows() {
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

__declspec(dllexport) void CloseMapping() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (hFileMapping == NULL || students == NULL) {
        printf("No mapping for closing.\n");
        return;
    }
    UnmapViewOfFile(students);
    CloseHandle(hFileMapping);
    students = NULL;
    hFileMapping = NULL;
    printf("Mapping closed.\n");
}