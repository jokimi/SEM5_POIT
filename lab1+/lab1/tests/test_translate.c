#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "translate.h"

void test_translate() {
    char test1[] = "hello";
    TranslateArray(test1, "lo", "xy");
    printf("Test 1 Result: %s (Expected: hexxy)\n", test1);
    assert(strcmp(test1, "hexxy") == 0);

    char test2[] = "world";
    TranslatePointer(test2, "ow", "ab");
    printf("Test 2 Result: %s (Expected: barld)\n", test2);
    assert(strcmp(test2, "barld") == 0);
}

void test_empty_strings() {
    char test3[] = "hello";
    TranslateArray(test3, "", "xy");
    printf("Test 3 Result: %s (Expected: hello)\n", test3);
    assert(strcmp(test3, "hello") == 0);

    char test4[] = "world";
    TranslatePointer(test4, "ow", "");
    printf("Test 4 Result: %s (Expected: world)\n", test4);
    assert(strcmp(test4, "world") == 0);
}

void test_null_pointers() {
    TranslateArray(NULL, "lo", "xy"); // Ошибка
    TranslatePointer(NULL, "ow", "ab"); // Ошибка
}

int main() {
    test_translate();
    test_empty_strings();
    test_null_pointers();
    return 0;
}