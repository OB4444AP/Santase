#include <iostream>
#include "strUtils.h"

int strLen(const char* str) {
    if (str == nullptr) {
        std::cout << "nullptr error";
        return -1;
    }
    int len = 0;

    while (*str != '\0') {
        len++;
        str++;
    }

    return len;
}

int strCompare(const char* str1, const char* str2) {
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

bool strIsNum(char* str) {
    if (str == nullptr) {
        std::cout << "nullptr error";
        return 0;
    }

    if (*str == '-') {
        str++;
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }
    
    return 1;
}

bool strIsPosNum(char* str) {
    if (str == nullptr) {
        std::cout << "nullptr error";
        return 0;
    }

    if (*str == '-') {
        return 0;
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }

    return 1;
}

bool strIsZero(char* str) {
    if (str == nullptr) {
        std::cout << "nullptr error";
        return 0;
    }

    return (str[0] == '0' && str[1] == '\0');
}

int strToNum(char* str) {
    if (str == nullptr) {
        std::cout << "nullptr error";
        return 0;
    }

    if (!strIsNum(str)) {
        std::cout << "This string isnt a number.";
        return 0;
    }

    int num = 0;
    int i = 0;
    bool isNegative = 0;

    if (*str == '-') {
        isNegative = 1;
        str++;
    }

    while (*str != '\0') {
        num += (*str - '0');
        num *= 10;
        str++;
    }
    num /= 10;

    if (isNegative) {
        return num * (-1);
    }
    return num;
}