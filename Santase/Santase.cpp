#include <iostream>
#include "strUtils.h"

struct Card {
    int suit;
    int value;
};

struct Player {
    Card hand[6];
};

struct Talon {
    Card talon[24];
};

int main()
{
    char str[10] = "Hello";
    char str2[10] = "yoyo";

    std::cout << strLen(str);

    return 0;
}