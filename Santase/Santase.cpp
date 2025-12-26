#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>;
#include "strUtils.h"

const int PLAYER_HAND = 6;
const int TALON_SIZE = 24;
const int COMMAND_MAX_SIZE = 50;
const size_t MAX_STR_LEN = 1024;

struct Card {
    int suit;// 1 - spades ; 2 - hearts ; 3 - diamonds ; 4 - clubs
    int value;
};

struct Player {
    Card* hand = new Card[PLAYER_HAND];
    int points = 0;
};

struct Talon {
    Card* talon = new Card[TALON_SIZE];
};

struct Settings {
    int pointsToWin = 11;
    int marriagePoints_nonTrump = 20;
    int marriagePoints_trump = 40;
    bool showPlayerPoints = 0;
    bool lastTrickBonus = 1;
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

Card pickTrumpSuit(Talon& talon) {
    Card c = drawCard(talon);
    talon.talon[(TALON_SIZE - 2 * PLAYER_HAND) - 1] = c;

    return c;
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
    BrightWhite = 15
};

void setColor(Color color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}

void printSuit(const Card c) {
    switch (c.suit) {
    case 1: {
        setColor(Color::BrightWhite);
        std::cout << "\xE2\x99\xA0";
        break;
    }
    case 2: {
        setColor(Color::LightRed);
        std::cout << "\xE2\x99\xA5";
        break;
    }
    case 3: {
        setColor(Color::LightRed);
        std::cout << "\xE2\x99\xA6";
        break;
    }
    case 4: {
        setColor(Color::BrightWhite);
        std::cout << "\xE2\x99\xA3";
        break;
    }
    }

    setColor(Color::White);
    return;
}

void printCard(const Card c) {
    switch (c.value) {
    case 1: {
        std::cout << 9;
        printSuit(c);
        break;
    }
    case 2: {
        std::cout << 10;
        printSuit(c);
        break;
    }
    case 3: {
        std::cout << 'J';
        printSuit(c);
        break;
    }
    case 4: {
        std::cout << 'Q';
        printSuit(c);
        break;
    }
    case 5: {
        std::cout << 'K';
        printSuit(c);
        break;
    }
    case 6: {
        std::cout <<'A';
        printSuit(c);
        break;
    }
    }
    
    return;
}

int getPointsToWin(Settings settings) {
    return settings.pointsToWin;
}

void changePointsToWin(Settings settings) {
    std::cout << "\nChange target points to win to: ";
    char strPoints[MAX_STR_LEN];
    std::cin >> strPoints;
    
    while (!strIsPosNum(strPoints) || strIsZero(strPoints)) {
        std::cout << "Invalid target points to win. Try again:";
        std::cin >> strPoints;
    }

    settings.pointsToWin = strToNum(strPoints);
    std::cout << "Succesfully changed target points to win [" << getPointsToWin(settings) << ']';
}

int getTrumpMarriagePoints(Settings settings) {
    return settings.marriagePoints_trump;
}

int getNonTrumpMarriagePoints(Settings settings) {
    return settings.marriagePoints_nonTrump;
}

void changeMarriagePoints(Settings settings) {
    std::cout << "\nChange non trump marriage points to: ";
    char nonTrumpMarriage[MAX_STR_LEN];
    std::cin >> nonTrumpMarriage;

    while (!strIsPosNum(nonTrumpMarriage) || strIsZero(nonTrumpMarriage)) {
        std::cout << "Invalid non trump marriage points. Try again:";
        std::cin >> nonTrumpMarriage;
    }

    std::cout << "\nChange trump marriage points to: ";
    char trumpMarriage[MAX_STR_LEN];
    std::cin >> trumpMarriage;

    while (!strIsPosNum(trumpMarriage) || strIsZero(trumpMarriage)) {
        std::cout << "Invalid trump marriage points. Try again:";
        std::cin >> trumpMarriage;
    }

    settings.marriagePoints_nonTrump = strToNum(nonTrumpMarriage);
    settings.marriagePoints_trump = strToNum(trumpMarriage);
    std::cout << "\nSuccesfully changed marriage points (non - trump / trump) [ ";
    std::cout << getNonTrumpMarriagePoints(settings) << " / " << getTrumpMarriagePoints(settings) << " ]";
}

void changeSettings(Settings settings) {
    std::cout << "\nSANTASE(66)\n";
    std::cout << "1) Target points to win [" << getPointsToWin(settings) << "]\n";
    std::cout << "2) Marriage points(non - trump / trump) [" << getNonTrumpMarriagePoints(settings);
    std::cout << " / " << getTrumpMarriagePoints(settings) << "]\n";
    std::cout << "3) Show players' points [on]\n";
    std::cout << "4) Last trick + 10 [on]\n";
    std::cout << "\nEnter number to change or press 0 to return:";

    char c;
    std::cin >> c;

    while (c != '0') {
        switch (c) {
        case '1': {
            changePointsToWin(settings);
            break;
        }
        case '2': {
            changeMarriagePoints(settings);
            break;
        }
        }

        std::cout << "\n\nEnter number to change or press 0 to return:";
        std::cin >> c;
    }

    commandIn(settings);
}


void gameStart(Settings settings) {
    Player p1, p2;
    Talon talon;

    initTalon(talon);
    deal(p1, p2, talon);    

    const Card TRUMP_CARD = pickTrumpSuit(talon);
}

void commandIn(Settings settings) {
    char command [COMMAND_MAX_SIZE];
    std::cin.getline(command, COMMAND_MAX_SIZE);

    char start[] = "start";
    if (strCompare(command, start) == 0) {
        gameStart(settings);
        return;
    }

    char settingsStr[] = "settings";
    if (strCompare(command, settingsStr) == 0) {
        changeSettings(settings);
        return;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(nullptr));
    Settings settings;

    commandIn(settings);

    return 0;
}