#include <iostream>
#include "strUtils.h"

int strLen(char* str) {
    int len = 0;

    while (*str != '\0') {
        len++;
    }

    return len;
}

int strCompare(char* str1, char* str2) {
    if (str1 == nullptr || str2 == nullptr) {
        std::cout << "nullptr error";
        return 0;
    }

    while (*str1 == *str2 && *str1 != '\0' && *str2 != '\0') {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}