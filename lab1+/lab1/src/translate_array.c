#include "translate.h"
#include <stdio.h>
#include <string.h>

void TranslateArray(char* s, const char* s1, const char* s2) {
    if (!s || !s1 || !s2) {
        fprintf(stderr, "Error: Null pointer argument\n");
        return;
    }
    if (s1[0] == '\0' || s2[0] == '\0') {
        fprintf(stderr, "Error: Empty strings are not allowed\n");
        return;
    }

    char temp[strlen(s) + 1];
    strcpy(temp, s);

    for (int i = 0; temp[i] != '\0'; i++) {
        for (int j = 0; s1[j] != '\0'; j++) {
            if (temp[i] == s1[j]) {
                temp[i] = s2[j % strlen(s2)]; // Обеспечивает цикличность s2
                break;
            }
        }
    }

    strcpy(s, temp);
}