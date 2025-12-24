#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>;
#include "strUtils.h"

const int PLAYER_HAND = 6;
const int TALON_SIZE = 24;
const int COMMAND_MAX_SIZE = 50;

struct Card {
    int suit;// 1 - spades ; 2 - hearts ; 3 - diamonds ; 4 - clubs
    int value;
};

struct Player {
    Card* hand = new Card[PLAYER_HAND];
};

struct Talon {
    Card* talon = new Card[TALON_SIZE];
};

Card drawCard(Talon& talon) {
    Card c = *(talon.talon);
    talon.talon++;
    return c;
}

bool isUniqueCard(const Card c, const Talon talon) {
    for (int i = 0; i<TALON_SIZE; i++) {
        if ((c.suit == talon.talon[i].suit) && (c.value == talon.talon[i].value)) {
            return false;
        }
    }
    return true;
}

void initTalon(Talon& talon) {
    for (int i = 0; i < TALON_SIZE; i++) {
        Card c;
        c.suit = (rand() % 4) + 1;
        c.value = (rand() % 6) + 1;
        
        while (!isUniqueCard(c, talon)) {
            c.suit = (rand() % 4) + 1;
            c.value = (rand() % 6) + 1;
        }
        talon.talon[i] = c;
    }
    return;
}

void deal(Player& p1, Player& p2, Talon& talon) {
    for (int i = 0; i < PLAYER_HAND; i++) {
        p1.hand[i] = drawCard(talon);
        p2.hand[i] = drawCard(talon);
    }
}

int pickTrumpSuit(Talon& talon) {
    Card c = drawCard(talon);
    talon.talon[(TALON_SIZE - 2 * PLAYER_HAND) - 1] = c;

    return c.suit;
}

char digToChar(int n) {
    if (n < 0 || n > 9) {
        std::cout << "digToChar error";
        return '?';
    }

    return n + '0';
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (color));
}

enum class Color
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Aqua = 3,
    Red = 4,
    Purple = 5,
    Yellow = 6,
    White = 7,
    Gray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightAqua = 11,
    LightRed = 12,
    LightPurple = 13,
    LightYellow = 14,
    BrightWhite
};

void printCard(const Card c) {
    char value = '0';

    switch (c.value) {
    case 11: {
        value = 'J';
        break;
    }
    case 12: {
        value = 'Q';
        break;
    }
    case 13: {
        value = 'K';
        break;
    }
    case 14: {
        value = 'A';
        break;
    }
    default: {
        value = digToChar(c.value);
        break;
    }
    }

    switch (c.suit) {
    case 1: {
        setColor(Color::BrightWhite);
        std::cout << value << "\xE2\x99\xA0";
        break;
    }
    case 2: {
        setColor(Color::LightRed);
        std::cout << value << "\xE2\x99\xA5";
        break;
    }
    case 3: {
        setColor(Color::LightRed);
        std::cout << value << "\xE2\x99\xA6";
        break;
    }
    case 4: {
        setColor(Color::BrightWhite);
        std::cout << value << "\xE2\x99\xA3";
        break;
    }
    }
    return;
}

void gameStart() {
    Player p1, p2;
    Talon talon;

    initTalon(talon);
    deal(p1, p2, talon);

    const int TRUMP_SUIT = pickTrumpSuit(talon);
    
}

void commandIn() {
    char command [COMMAND_MAX_SIZE];
    std::cin.getline(command, COMMAND_MAX_SIZE);

    char start[6] = "start";
    if (strCompare(command, start) == 0) {
        gameStart();
        return;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(nullptr));

    commandIn();

    return 0;
}