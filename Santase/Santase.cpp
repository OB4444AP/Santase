#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>;
#include "strUtils.h"

const int COMMAND_MAX_SIZE = 50;
const size_t MAX_STR_LEN = 1024;

struct Card {
    int suit;// 1 - spades ; 2 - hearts ; 3 - diamonds ; 4 - clubs
    int value;
};

struct Player {
    int name = 0;
    int points = 0;
    int handSize = 6;
    Card* hand = new Card[handSize];
};

struct Talon {
    int talonSize = 24;
    Card* talon = new Card[talonSize];
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
    talon.talonSize--;
    return c;
}

void sortHand(Player& p) {
    for (int i = 0; i < p.handSize - 1; i++) {
        for (int j = 0; j < p.handSize - i - 1; j++) {
            if (p.hand[j].value < p.hand[j + 1].value) {
                Card c = p.hand[j + 1];
                p.hand[j + 1] = p.hand[j];
                p.hand[j] = c;
            }
        }
    }
    for (int i = 0; i < p.handSize - 1; i++) {
        for (int j = 0; j < p.handSize - i - 1; j++) {
            if (p.hand[j].suit > p.hand[j + 1].suit) {
                Card c = p.hand[j + 1];
                p.hand[j + 1] = p.hand[j];
                p.hand[j] = c;
            }
        }
    }
}

bool correctCardIndex(const Player p, const int index) {
    if (index > p.handSize - 1) {
        std::cout << "Invalid command or index." << std::endl;
        return false;
    }

    return true;
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
        std::cout << 'A';
        printSuit(c);
        break;
    }
    }

    return;
}

Card playCard(Player& p, Talon& talon, int index) {
    Card c = p.hand[index];

    for (int i = index; i < p.handSize - 1; i++) {
        Card t = p.hand[i];
        p.hand[i] = p.hand[i + 1];
        p.hand[i + 1] = t;
    }

    p.hand[p.handSize - 1] = drawCard(talon);

    std::cout << std::endl;
    std::cout << "P" << p.name << " played ";
    printCard(c);
    std::cout << std::endl;

    return c;
}

void printHand(const Player p) {
    std::cout << "[ ";
    for (int i = 0; i < p.handSize; i++) {
        printCard(p.hand[i]);
        std::cout << " ";
    }
    std::cout << ']';
}

bool isUniqueCard(const Card c, const Talon talon) {
    for (int i = 0; i<talon.talonSize; i++) {
        if ((c.suit == talon.talon[i].suit) && (c.value == talon.talon[i].value)) {
            return false;
        }
    }
    return true;
}

void initTalon(Talon& talon) {
    for (int i = 0; i < talon.talonSize; i++) {
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
    for (int i = 0; i < p1.handSize; i++) {
        p1.hand[i] = drawCard(talon);
        p2.hand[i] = drawCard(talon);
    }

    sortHand(p1);
    sortHand(p2);
}

Card pickTrumpSuit(Talon& talon) {
    Card c = drawCard(talon);
    talon.talon[talon.talonSize - 1] = c;

    return c;
}

int getPointsToWin(const Settings settings) {
    return settings.pointsToWin;
}

void changePointsToWin(Settings settings) {
    std::cout << "\nChange target points to win to: ";
    char strPoints[MAX_STR_LEN];
    std::cin >> strPoints;
    
    while (!strIsPosNum(strPoints) || strIsZero(strPoints)) {
        std::cout << "Invalid target points to win. Try again: ";
        std::cin >> strPoints;
    }

    settings.pointsToWin = strToNum(strPoints);
    std::cout << "Successfully changed target points to win [" << getPointsToWin(settings) << ']';
}

int getTrumpMarriagePoints(const Settings settings) {
    return settings.marriagePoints_trump;
}

int getNonTrumpMarriagePoints(const Settings settings) {
    return settings.marriagePoints_nonTrump;
}

void changeMarriagePoints(Settings settings) {
    std::cout << "\nChange non trump marriage points to: ";
    char nonTrumpMarriage[MAX_STR_LEN];
    std::cin >> nonTrumpMarriage;

    while (!strIsPosNum(nonTrumpMarriage) || strIsZero(nonTrumpMarriage)) {
        std::cout << "Invalid non trump marriage points. Try again: ";
        std::cin >> nonTrumpMarriage;
    }

    std::cout << "\nChange trump marriage points to: ";
    char trumpMarriage[MAX_STR_LEN];
    std::cin >> trumpMarriage;

    while (!strIsPosNum(trumpMarriage) || strIsZero(trumpMarriage)) {
        std::cout << "Invalid trump marriage points. Try again: ";
        std::cin >> trumpMarriage;
    }

    settings.marriagePoints_nonTrump = strToNum(nonTrumpMarriage);
    settings.marriagePoints_trump = strToNum(trumpMarriage);
    std::cout << "\nSuccessfully changed marriage points (non - trump / trump) [ ";
    std::cout << getNonTrumpMarriagePoints(settings) << " / " << getTrumpMarriagePoints(settings) << " ]";
}

bool getShowPlayerPoints(const Settings settings){
    return settings.showPlayerPoints;
}

void printShowPlayerPoints(const Settings settings) {
    if (settings.showPlayerPoints) {
        std::cout << "on";
        return;
    }

    std::cout << "off";
    return;
}

void changeShowPlayerPoints(Settings settings) {
    std::cout << "\nChange show player points (on / off): ";
    char showPlayerPoints[MAX_STR_LEN];
    std::cin >> showPlayerPoints;

    char on[] = "on";
    char off[] = "off";
    int playerPoints = 0;
    while (true) {
        if (strCompare(showPlayerPoints, on) == 0) {
            playerPoints = 1;
            break;
        }
        if (strCompare(showPlayerPoints, off) == 0) {
            playerPoints = 0;
            break;
        }

        std::cout << "Invalid input. Try \"on\" or \"off\" : ";
        std::cin >> showPlayerPoints;
    }

    settings.showPlayerPoints = playerPoints;
    std::cout << "Successfully turned show player points ";
    printShowPlayerPoints(settings);
    return;
}

void printLastTrickBonus(const Settings settings) {
    if (settings.lastTrickBonus) {
        std::cout << "on";
        return;
    }

    std::cout << "off";
    return;
}

void changeLastTrickBonus(Settings settings) {
    std::cout << "\nChange last trick +10 (on / off): ";
    char lastTrickBonus[MAX_STR_LEN];
    std::cin >> lastTrickBonus;

    char on[] = "on";
    char off[] = "off";
    bool lastTrick = 0;

    while (true) {
        if (strCompare(lastTrickBonus, on) == 0) {
            lastTrick = 1;
            break;
        }
        if (strCompare(lastTrickBonus, off) == 0) {
            lastTrick = 0;
            break;
        }

        std::cout << "Invalid input. Try \"on\" or \"off\" : ";
        std::cin >> lastTrickBonus;
    }

    settings.lastTrickBonus = lastTrick;
    std::cout << "Successfully turned last trick +10 ";
    printLastTrickBonus(settings);
    return;
}

void changeSettings(Settings settings) {
    std::cout << "\nSANTASE(66)\n";
    std::cout << "1) Target points to win [" << getPointsToWin(settings) << "]\n";
    std::cout << "2) Marriage points(non - trump / trump) [" << getNonTrumpMarriagePoints(settings);
    std::cout << " / " << getTrumpMarriagePoints(settings) << "]\n";
    std::cout << "3) Show players' points [";
    printShowPlayerPoints(settings);
    std::cout << "]\n";
    std::cout << "4) Last trick + 10 [";
    printLastTrickBonus(settings);
    std::cout << "]\n";
    std::cout << "\nEnter number to change or press 0 to return: ";

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
        case '3': {
            changeShowPlayerPoints(settings);
            break;
        }
        case '4': {
            changeLastTrickBonus(settings);
            break;
        }
        }

        std::cout << "\n\nEnter number to change or press 0 to return: ";
        std::cin >> c;
    }
}

Card* playerCommand(const Settings settings, Player p, Talon talon) {
    char command[COMMAND_MAX_SIZE];

    while (true) {
        std::cout << "> ";
        std::cin.getline(command, COMMAND_MAX_SIZE - 1);

        char hand[] = "hand";
        if (strCompare(command, hand) == 0) {
            sortHand(p);
            printHand(p);
            std::cout << std::endl;
            continue;
        }

        char play[] = "play ";
        if (startsWith(command, play) && strLen(command) == strLen(play) + 1) {
            int lastIndex = strLen(command) - 1;
            int index = charToNum(command[lastIndex]);
            if (!correctCardIndex(p, index)) {
                continue;
            }
            Card c = playCard(p, talon, index);
            return &c;
        }
        else {
            std::cout << "Invalid command or index." << std::endl;
            continue;
        }
    }
}

void gameStart(const Settings settings) {
    Player p1, p2;
    p1.name = 1;
    p2.name = 2;
    Talon talon;

    initTalon(talon);
    deal(p1, p2, talon);    

    const Card TRUMP_CARD = pickTrumpSuit(talon);

    Player inPlay;

    while (true) {
        inPlay = p1;
        Card* c1ptr = playerCommand(settings, inPlay, talon);
        if (c1ptr != nullptr) {
            inPlay = p2;
            Card* c2ptr = playerCommand(settings, inPlay, talon);
        }
    }
}

void printRules(const Settings settings) {
    std::cout << "\nSANTASE (66)\nEach player gets 6 cards.The Trump suit is chosen at random.\nCard values : A = 11, 10 = 10, K = 4, Q = 3, J = 2, 9 = 0.\n";
    std::cout << "A marriage (K + Q of the same suit) gives " << getNonTrumpMarriagePoints(settings) << " points, or ";
    std::cout << getTrumpMarriagePoints(settings) << " if the trump suit.\n";
    std::cout << "The first player to reach 66 points wins the round.\n";
    std::cout << "At the start of the game the rules are relaxed.\n";
    std::cout << "This means both players can play any card of their choice and they are not obliged to follow suit.\n";
    std::cout << "The highest card of the led suit wins. If only one player played a trump he wins the trick no matter the value of the\n";
    std::cout << "trump card.\n";
    std::cout << "The player that wins the trick gets the points of the two cards played added to their total points.\n";
    std::cout << "Then both players draw a card (if possible) so they both have 6 cards again.\n";
    std::cout << "The player that won the trick draws first.\n";
    std::cout << "If a player has the 9 of Trumps and it is their turn they can swap it for the bottom card in the talon.\n";
    std::cout << "If a player has both the King and Queen of the same suit\n";
    std::cout << "and they have won at least one trick and it is their turn\n";
    std::cout << "they can play one of them and declare a marriage.\n\n";
    std::cout << "If a player decides to close the talon or the talon runs out of cards the rules become strict.\n";
    std::cout << "A player can close the talon any time its their turn.\n\n";
    std::cout << "Players must now follow suits (if your opponent plays a Spade you must also play a Spade).\n";
    std::cout << "Players must win the trick if possible. They have to play a higher card than their opponent if they can.\n";
    std::cout << "If a player cannot follow suit they must play a trump card.\n";
    std::cout << "If a player cannot follow suit or trump they can play any card they want.\n";
    std::cout << "Players do not draw cards after tricks.\n\n";
    std::cout << "If a player thinks they have reached 66 points they can announce it any time during the game.\n";
    std::cout << "When a player announces they have 66 points or both players run out of cards scoring begins.\n\n";
    std::cout << "If the player who announced \"66\" has at least 66 points the points are as follows:\n";
    std::cout << "1 Point: If the opponent has 33 points or more.\n";
    std::cout << "2 Points: If the opponent has 32 points or fewer, but won at least one trick.\n";
    std::cout << "3 Points: If the opponent won 0 tricks.\n";
    std::cout << "If the player who announces \"66\" has less than 66 points he gets 0 points.\n";
    std::cout << "His opponent gets 2 points instead. Or 3 points if the announcing player has won 0 tricks.\n";
    std::cout << "If a player closes the talon and fails to get to 66 points their opponent gets 2 points\n";
    std::cout << "or 3 if the closing player has won 0 tricks.\n";
    std::cout << "When the talon has no cards and both players play their last cards the winning player gets +10 extra points.\n";
    std::cout << "If the game is played to its end and neither player announced \"66\" or closes the talon\n";
    std::cout << "the player with more points wins 1 point. In case of a tie the player who won the last trick wins 1 point.";
}

void commandIn(Settings settings) {
    char command[COMMAND_MAX_SIZE];
    char start[] = "start";

    do {
        std::cout << "> ";
        std::cin.getline(command, COMMAND_MAX_SIZE - 1);

        if (strCompare(command, start) == 0) {
            gameStart(settings);
        }

        char settingsStr[] = "settings";
        if (strCompare(command, settingsStr) == 0) {
            changeSettings(settings);
        }

        char rules[] = "rules";
        if (strCompare(command, rules) == 0) {
            printRules(settings);
            std::cout << std::endl;
        }

        if (strCompare(command, rules) != 0 && strCompare(command, settingsStr) != 0 && strCompare(command, start) != 0) {
            std::cout << "Invalid command. Try again: ";
        }
    } while (strCompare(command, start) != 0);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(nullptr));
    Settings settings;

    commandIn(settings);

    return 0;
}