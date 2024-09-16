#include "translate.h"
#include <stdio.h>
#include <string.h>

void TranslatePointer(char* s, const char* s1, const char* s2) {
    if (!s || !s1 || !s2) {
        fprintf(stderr, "Error: Null pointer argument\n");
        return;
    }
    if (s1[0] == '\0' || s2[0] == '\0') {
        fprintf(stderr, "Error: Empty strings are not allowed\n");
        return;
    }

    char temp[strlen(s) + 1]; // Временная строка для хранения результата
    strcpy(temp, s);

    for (char* p = temp; *p != '\0'; p++) {
        for (const char* q = s1; *q != '\0'; q++) {
            if (*p == *q) {
                *p = s2[(q - s1) % strlen(s2)]; // Обеспечивает цикличность s2
                break;
            }
        }
    }

    strcpy(s, temp);
}