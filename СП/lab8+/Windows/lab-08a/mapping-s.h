#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <windows.h>

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

void OpenMapping(const char* filePath, int size);
void AddRow(int pos);
void RemRow(int pos);
void PrintRow(int pos);
void PrintRows();
void CloseMapping();

#endif