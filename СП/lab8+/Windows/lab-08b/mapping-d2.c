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
        printf("������: ������� ��� ������.\n");
        return;
    }
    HANDLE hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("������: �� ������� ������� ����.\n");
        return;
    }
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        printf("������ ����� ���� � �������� %d ����.\n", sizeof(struct Student) * size);
        fileSize = sizeof(struct Student) * size;
        SetFilePointer(hFile, fileSize, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
    }
    hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    CloseHandle(hFile);
    if (hFileMapping == NULL) {
        printf("������ ��� �������� ��������.\n");
        return;
    }
    students = (struct Student*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (students == NULL) {
        printf("������ ��� ����������� �����.\n");
        CloseHandle(hFileMapping);
        hFileMapping = NULL;
        return;
    }
    // ������������� ����� ������, ���� ��� ����� ����
    for (int i = 0; i < size; i++) {
        memset(&students[i], 0, sizeof(struct Student));
    }
    FlushViewOfFile(students, sizeof(struct Student) * size);
    listSize = size;
    printf("������� ������� ������. ������ ������: %d\n", listSize);
}

__declspec(dllexport) void AddRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
        printf("������: ������� �� ������.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("������: ������������ �������. ���������� ������� �� 0 �� %d.\n", listSize - 1);
        return;
    }
    if (strlen(students[pos].Name) > 0) {
        printf("������: ������� %d ��� ������.\n", pos);
        return;
    }
    struct Student newStudent;
    memset(&newStudent, 0, sizeof(newStudent)); // ������� ������ ��� ����� ������
    printf("������� ���: ");
    fgets(newStudent.Name, sizeof(newStudent.Name), stdin);
    newStudent.Name[strcspn(newStudent.Name, "\n")] = '\0';
    printf("������� �������: ");
    fgets(newStudent.Surname, sizeof(newStudent.Surname), stdin);
    newStudent.Surname[strcspn(newStudent.Surname, "\n")] = '\0';
    printf("������� ����: ");
    if (scanf("%hhu", &newStudent.Course) != 1) {
        printf("������: ������������ ���� �����.\n");
        getchar();
        return;
    }
    printf("������� ������: ");
    if (scanf("%hhu", &newStudent.Group) != 1) {
        printf("������: ������������ ���� ������.\n");
        getchar();
        return;
    }
    printf("������� ID: ");
    if (scanf("%s", newStudent.ID) != 1) {
        printf("������: ������������ ���� ID.\n");
        getchar();
        return;
    }
    getchar();
    students[pos] = newStudent;
    if (!FlushViewOfFile(students, sizeof(struct Student) * listSize)) {
        printf("������: �� ������� ��������� ��������� � ����.\n");
    }
    else {
        printf("������� �������� �� ������� %d.\n", pos);
    }
}

__declspec(dllexport) void RemRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
        printf("������: ������� �� ������.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("������: ������������ �������. ���������� ������� �� 0 �� %d.\n", listSize - 1);
        return;
    }
    if (strlen(students[pos].Name) == 0) {
        printf("������: ������� %d �����.\n", pos);
        return;
    }
    memset(&students[pos], 0, sizeof(struct Student));
    FlushViewOfFile(students, sizeof(struct Student) * listSize);
    printf("������� �� ������� %d �����.\n", pos);
}

__declspec(dllexport) void PrintRow(int pos) {
    if (hFileMapping == NULL || students == NULL) {
        printf("������: ������� �� ������.\n");
        return;
    }
    if (pos < 0) {
        pos = listSize + pos;
    }
    if (pos < 0 || pos >= listSize) {
        printf("������: ������������ �������. ���������� ������� �� 0 �� %d.\n", listSize - 1);
        return;
    }

    struct Student student = students[pos];
    if (strlen(student.Name) == 0) {
        printf("������� %d �����.\n", pos);
    }
    else {
        printf("������� %d:\n", pos);
        printf("\t���: %s\n", student.Name);
        printf("\t�������: %s\n", student.Surname);
        printf("\t����: %d\n", student.Course);
        printf("\t������: %d\n", student.Group);
        printf("\tID: %s\n", student.ID);
    }
}

__declspec(dllexport) void PrintRows() {
    if (hFileMapping == NULL || students == NULL) {
        printf("������: ������� �� ������.\n");
        return;
    }

    for (int i = 0; i < listSize; i++) {
        printf("������� %d: ", i);
        if (strlen(students[i].Name) == 0) {
            printf("�����\n");
        }
        else {
            printf("\n");
            printf("\t���: %s\n", students[i].Name);
            printf("\t�������: %s\n", students[i].Surname);
            printf("\t����: %d\n", students[i].Course);
            printf("\t������: %d\n", students[i].Group);
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