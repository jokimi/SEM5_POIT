#include <stdio.h>
#include "translate.h"

int main() {
    char str[] = "hello world";
    char s1[] = "lo";
    char s2[] = "xy";

    TranslateArray(str, s1, s2);
    printf("Translated (Array): %s\n", str);

    char str2[] = "hello world";
    TranslatePointer(str2, s1, s2);
    printf("Translated (Pointer): %s\n", str2);

    return 0;
}