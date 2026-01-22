#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>;
#include "strUtils.h"

const int COMMAND_MAX_SIZE = 50;
const size_t MAX_STR_LEN = 1024;
const int MAX_HAND_SIZE = 6;
const int MAX_TALON_SIZE = 24;
const int LAST_TRICK_BONUS = 10;

struct Card {
    int suit;// 1 - spades ; 2 - hearts ; 3 - diamonds ; 4 - clubs
    int value;
};

struct Player {
    int name = 0;
    int trickPoints = 0;
    int gamePoints = 0;
    int handSize = MAX_HAND_SIZE;
    Card* hand = nullptr;
    Card cardPlayed = { 0,0 };
    Card lastTrickCard = { 0,0 };
    bool hasSurrendered = false;
    bool hasSurrenderedForever = false;
    bool hasEnded = false;
    int tricksWon = 0;
    int roundsWon = 0;
};

struct Round {
    Player winner;
    int pointsWon = 0;
    Player p1;
    Player p2;
};

struct Talon {
    int talonSize = MAX_TALON_SIZE;
    Card* talon = nullptr;
    Card trumpCard;
    bool isClosed = false;
    Player lastTrickWinner;
};

struct Settings {
    int pointsToWin = 11;
    int marriagePoints_nonTrump = 20;
    int marriagePoints_trump = 40;
    bool showPlayerPoints = 0;
    bool lastTrickBonus = 1;
};

int getCardPoints(Card c) {
    switch (c.value) {
    case 1: return 0;
        break;
    case 2:return 10;
        break;
    case 3:return 2;
        break;
    case 4:return 3;
        break;
    case 5:return 4;
        break;
    case 6:return 11;
        break;
    default:return 0;
        break;
    }
}

Player roundWinner(Player& p1, Player& p2) {
    if (p1.trickPoints == p2.trickPoints) {
        Player tie = p1;
        tie.name = 0;
        return tie;
    }

    if (p1.trickPoints >= 66 && p1.hasEnded) {
        if (p2.tricksWon == 0) {
            p1.gamePoints += 3;
        }
        else if (p2.trickPoints <= 32) {
            p1.gamePoints += 2;
        }
        else if (p2.trickPoints >= 33) {
            p1.gamePoints += 1;
        }
        p1.roundsWon++;
        return p1;
    }
    if (p1.trickPoints < 66 && p1.hasEnded) {
        if (p1.tricksWon == 0) {
            p2.gamePoints += 3;
        }
        else if (p1.trickPoints > 0) {
            p2.gamePoints += 2;
        }
        p2.roundsWon++;
        return p2;
    }
    if (p2.trickPoints >= 66 && p2.hasEnded) {
        if (p1.tricksWon == 0) {
            p2.gamePoints += 3;
        }
        else if (p1.trickPoints <= 32) {
            p2.gamePoints += 2;
        }
        else if (p1.trickPoints >= 33) {
            p2.gamePoints += 1;
        }
        p2.roundsWon++;
        return p2;
    }
    if (p2.trickPoints < 66 && p2.hasEnded) {
        if (p2.tricksWon == 0) {
            p1.gamePoints += 3;
        }
        else if (p2.trickPoints > 0) {
            p1.gamePoints += 2;
        }
        p1.roundsWon++;
        return p1;
    }
    if (p1.trickPoints > p2.trickPoints) {
        p1.gamePoints++;
        p1.roundsWon++;
        return p1;
    }
    else {
        p2.gamePoints++;
        p2.roundsWon++;
        return p2;
    }
}

bool sameSuit(Card c1, Card c2) {
    return c1.suit == c2.suit;
}

bool talonIsEmpty(Talon& talon) {
    return talon.talonSize == 0;
}

Card drawCard(Talon& talon) {
    int index = MAX_TALON_SIZE - talon.talonSize;

    Card c = talon.talon[index];

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

int getSuitVal(const char suit) {
    switch (suit) {
    case 'S':return 1;
        break;
    case 'H':return 2;
        break;
    case 'D':return 3;
        break;
    case 'C':return 4;
        break;
    default:return -1;
        break;
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

bool isKingOrQueenOfSuit(const Player& p, const int index, const int suit) {
    return p.hand[index].suit == suit && (p.hand[index].value == 4 || p.hand[index].value == 5);
}

Card getCard(const Player p, const int index) {
    return p.hand[index];
}

Card playCard(Player& p, Talon& talon, const int index) {
    Card c = p.hand[index];

    for (int i = index; i < p.handSize - 1; i++) {
        Card t = p.hand[i];
        p.hand[i] = p.hand[i + 1];
        p.hand[i + 1] = t;
    }

    if (talon.isClosed || talonIsEmpty(talon)) {
        p.handSize--;
    }

    std::cout << std::endl;
    std::cout << "P" << p.name << " played ";
    printCard(c);
    std::cout << std::endl;

    return c;
}

int compareCards(const Card c1, const Card c2, const Talon talon) {
    if (c1.suit == talon.trumpCard.suit && c2.suit != talon.trumpCard.suit) {
        return -1;
    }
    if (c2.suit == talon.trumpCard.suit && c1.suit != talon.trumpCard.suit) {
        return 1;
    }
    if (c1.suit != c2.suit) {
        return -1;
    }
    if (c1.value > c2.value) {
        return -1;
    }
    if (c1.value < c2.value) {
        return 1;
    }
    return 0;
}

Player trickWinner(const Settings settings, Player& p1, Player& p2, Talon& talon) {
    if (compareCards(p1.cardPlayed, p2.cardPlayed, talon) == -1) {
        std::cout << "P" << p1.name << " won the trick!" << std::endl;

        talon.lastTrickWinner = p1;
        p1.tricksWon++;
        p1.lastTrickCard = p1.cardPlayed;
        p2.lastTrickCard = p2.cardPlayed;

        p1.trickPoints += getCardPoints(p1.cardPlayed) + getCardPoints(p2.cardPlayed);

        return p1;
    }
    std::cout << "P" << p2.name << " won the trick!" << std::endl;

    talon.lastTrickWinner = p2;
    p2.tricksWon++;
    p1.lastTrickCard = p1.cardPlayed;
    p2.lastTrickCard = p2.cardPlayed;

    p2.trickPoints += getCardPoints(p1.cardPlayed) + getCardPoints(p2.cardPlayed);

    return p2;
}

void printHand(const Player p) {
    std::cout << "[ ";
    for (int i = 0; i < p.handSize; i++) {
        printCard(p.hand[i]);
        std::cout << " ";
    }
    std::cout << ']';
}

void printTurnMessage(const Player p, const Talon talon) {
    std::cout << std::endl << "P" << p.name << "'s turn:" << std::endl;
    std::cout << "Hand: ";
    printHand(p);
    std::cout << std::endl << "Trump suit: ";
    printSuit(talon.trumpCard);
    std::cout << std::endl << "Bottom card: ";
    printCard(talon.trumpCard);
    std::cout << std::endl << "Cards left in deck: " << talon.talonSize << std::endl;
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
    int topCardInd = MAX_TALON_SIZE - talon.talonSize;
    int bottomCardInd = MAX_TALON_SIZE - 1;

    Card c = talon.talon[topCardInd];
    talon.talon[topCardInd] = talon.talon[bottomCardInd];
    talon.talon[bottomCardInd] = c;
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
    std::cin.ignore();

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

bool switchNine(Player& p, Talon& talon) {
    if (p.tricksWon == 0 || talon.isClosed || talon.talonSize <= 2) {
        return false;
    }
    for (int i = 0; i < p.handSize; i++) {
        if (p.hand[i].suit == talon.trumpCard.suit && p.hand[i].value == 1) {
            Card t = p.hand[i];

            std::cout << std::endl << "You exchanged ";
            printCard(t);
            std::cout << " for ";
            printCard(talon.trumpCard);
            std::cout << std::endl;

            p.hand[i] = talon.trumpCard;
            talon.trumpCard = t;

            talon.talon[MAX_TALON_SIZE - 1] = t;
            return true;
        }
    }
    return false;
}

int marriageF(Player& p, const char suit, Talon& talon, Settings settings) {
    if (p.name != talon.lastTrickWinner.name || p.tricksWon < 1) {
        std::cout << std::endl << "You must have at least 1 trick won and it must be your turn for declare a marriage." << std::endl;
        return 0;
    }
    int suitVal = getSuitVal(suit);

    int kingInd = -1, queenInd = -1;

    for (int i = 0; i < p.handSize; i++) {
        if (p.hand[i].suit == suitVal && p.hand[i].value == 5) {
            kingInd = i;
        }
        if (p.hand[i].suit == suitVal && p.hand[i].value == 4) {
            queenInd = i;
        }
    }

    if (kingInd != -1 && queenInd != -1) {
        std::cout << std::endl << "Marriage declared: ";
        printCard(p.hand[kingInd]);
        std::cout << " + ";
        printCard(p.hand[queenInd]);
    }

    if (suitVal == talon.trumpCard.value) {
        std::cout << " (trump suit)" << std::endl << "You earned " << settings.marriagePoints_trump << " points." << std::endl;
        p.trickPoints += settings.marriagePoints_trump;
        std::cout << "You must play ";
        printCard(p.hand[kingInd]);
        std::cout << " or ";
        printCard(p.hand[queenInd]);
        std::cout << std::endl;
    }
    else {
        std::cout << std::endl << "You earned " << settings.marriagePoints_nonTrump << " points." << std::endl;
        p.trickPoints += settings.marriagePoints_nonTrump;
        std::cout << "You must play ";
        printCard(p.hand[kingInd]);
        std::cout << " or ";
        printCard(p.hand[queenInd]);
        std::cout << std::endl;
    }
    return suitVal;
}

void closeTalon(Talon& talon) {
    talon.isClosed = true;

    std::cout << "Stock closed. No more cards will be drawn." << std::endl;
    std::cout << "Strict rules are now in effect" << std::endl;
}

void printLastTrick(const Player p1, const Player p2, const Talon talon) {
    if (talon.lastTrickWinner.name == 0) {
        std::cout << std::endl << "No trick played yet." << std::endl;
        return;
    }

    std::cout << std::endl << "Player P" << p1.name << ": ";
    printCard(p1.lastTrickCard);
    std::cout << std::endl << "Player P" << p2.name << ": ";
    printCard(p2.lastTrickCard);
    std::cout << std::endl << "Winner: Player " << talon.lastTrickWinner.name << std::endl;
}

void printTrump(const Talon talon) {
    std::cout << std::endl << "Trump suit: ";
    printSuit(talon.trumpCard);
    std::cout << std::endl;
}

void printHistory(const Round* rounds, const int roundsPlayed) {
    std::cout << std::endl;
    for (int i = 0; i < roundsPlayed - 1; i++) {
        if (rounds[i].winner.name != 0) {
            std::cout << std::endl << "Round " << i + 1 << ": Winner - Player " << rounds[i].winner.name << " (+" << rounds[i].pointsWon << ") | ";
            std::cout << "Player " << rounds[i].p1.name << ": " << rounds[i].p1.trickPoints << " points | Player " << rounds[i].p2.name << ": " << rounds[i].p2.trickPoints << " points";
        }
        else {
            std::cout << std::endl << "Round " << i + 1 << ": Tie" << " | Player 1" << ": " << rounds[i].p1.trickPoints << " points | Player 2: " << rounds[i].p2.trickPoints << " points";
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << "Round " << roundsPlayed << ": Ongoing" << std::endl;
}

bool trickFinished(const Player p1, const Player p2) {
    if (p1.hasEnded || p2.hasEnded || p1.hasSurrendered || p2.hasSurrendered || (p1.handSize == 0 && p2.handSize == 0)) {
        return true;
    }
    return false;
}

bool gameFinished(const Settings settings, const Player p1, const Player p2) {
    if (p1.gamePoints != p2.gamePoints && p1.gamePoints >= settings.pointsToWin || p2.gamePoints >= settings.pointsToWin) {
        return true;
    }
    if (p1.hasSurrenderedForever || p2.hasSurrenderedForever) {
        return true;
    }
    return false;
}

bool isMatched(const Player& p1, const Player& p2, const Card c, const Talon& talon) {
    if (c.suit == p1.cardPlayed.suit) {
        return true;
    }

    bool hasTrump = false;
    bool hasSuit = false;

    for (int i = 0; i < p2.handSize; i++) {
        if (p2.hand[i].suit == talon.trumpCard.suit) {
            hasTrump = true;
        }
        if (p2.hand[i].suit == p1.cardPlayed.suit) {
            hasSuit = true;
        }
    }
    if (hasSuit) {
        return false;
    }
    if (c.suit == talon.trumpCard.suit) {
        return true;
    }
    if (hasTrump) {
        return false;
    }
    else return true;
}

void printStatus(const Player p1, const Player p2, const Talon talon) {
    std::cout << std::endl << "Player " << p1.name << " has won " << p1.roundsWon << " rounds." << std::endl;
    std::cout << "Player " << p2.name << " has won " << p2.roundsWon << " rounds." << std::endl;
    printTrump(talon);
    std::cout << std::endl << "Cards remaining in talon: " << talon.talonSize << std::endl;
    std::cout << std::endl << "Bottom card: ";
    printCard(talon.trumpCard);
    std::cout << std::endl << std::endl;
}

void stopsRound(Player& p) {
    p.hasEnded = 1;
}

void surrenders(Player& p1, Player& p2) {
    p1.hasSurrendered = true;
    p2.trickPoints = 66;

    std::cout << std::endl << "Player " << p1.name << " has surrendered." << std::endl;
}

void surrendersForever(Player& p) {
    p.hasSurrenderedForever = true;

    std::cout << std::endl << "Player " << p.name << " has surrender forever.";
}

Player playerCommand(const Settings& settings, Player& inPlay, Player& outOfPlay, Talon& talon, const Round*& rounds, const int& roundsPlayed) {
    char command[COMMAND_MAX_SIZE];
    int marriageSuit = 0;

    while (true) {
        printTurnMessage(inPlay, talon);

        std::cout << "> ";
        std::cin.getline(command, COMMAND_MAX_SIZE - 1);

        const char hand[] = "hand";
        if (strCompare(command, hand) == 0) {
            sortHand(inPlay);
            printHand(inPlay);
            std::cout << std::endl;
            continue;
        }

        const char play[] = "play ";
        if (startsWith(command, play) && strLen(command) == strLen(play) + 1) {
            int lastIndex = strLen(command) - 1;
            int index = charToNum(command[lastIndex]);

            if (!correctCardIndex(inPlay, index)) {
                continue;
            }
            if (marriageSuit != 0 && !isKingOrQueenOfSuit(inPlay, index, marriageSuit)) {
                std::cout << std::endl << "Play the King or Queen of the chosen suit." << std::endl;
                continue;
            }
            if ((talon.isClosed || talon.talonSize == 0) && talon.lastTrickWinner.name != inPlay.name && !isMatched(outOfPlay, inPlay, getCard(inPlay, index), talon)) {
                std::cout << std::endl << "Must match suit or trick." << std::endl;
                continue;
            }
            inPlay.cardPlayed = playCard(inPlay, talon, index);
            return inPlay;
        }

        const char switch9[] = "switch9";
        if (strCompare(command, switch9) == 0) {
            if (!switchNine(inPlay, talon) || talon.lastTrickWinner.name != inPlay.name) {
                std::cout << std::endl << "Cannot switch nine" << std::endl;
            }
            continue;
        }

        const char marriage[] = "marriage ";
        if (startsWith(command, marriage) && strLen(command) == strLen(marriage) + 1) {
            int lastIndex = strLen(command) - 1;
            char suit = command[lastIndex];
            marriageSuit = marriageF(inPlay, suit, talon, settings);
            if (!marriageSuit) {
                std::cout << std::endl << "Cannot declare marriage." << std::endl;
            }
            continue;
        }

        const char close[] = "close";
        if (strCompare(command, close) == 0) {
            closeTalon(talon);
            continue;
        }

        const char lastTrick[] = "last-trick";
        if (strCompare(command, lastTrick) == 0) {
            printLastTrick(inPlay, outOfPlay, talon);
            continue;
        }

        const char trump[] = "trump";
        if (strCompare(command, trump) == 0) {
            printTrump(talon);
            continue;
        }

        const char history[] = "history";
        if (strCompare(command, history) == 0) {
            printHistory(rounds, roundsPlayed);
            continue;
        }

        const char status[] = "status";
        if (strCompare(command, status) == 0) {
            printStatus(inPlay, outOfPlay, talon);
            continue;
        }

        const char stop[] = "stop";
        if (strCompare(command, stop) == 0) {
            stopsRound(inPlay);
            return inPlay;
        }

        const char surrender[] = "surrender";
        if (strCompare(command, surrender) == 0) {
            surrenders(inPlay, outOfPlay);
            return inPlay;
        }

        const char surrenderForever[] = "surrender-forever";
        if (strCompare(command, surrender) == 0) {
            surrendersForever(inPlay);
        }

        std::cout << std::endl << "Invalid command or index." << std::endl;
        continue;
    }
}

Round* increaseRounds(Round* rounds, int& roundsPlayed) {
    roundsPlayed++;
    Round* result = new Round[roundsPlayed];

    for (int i = 0; i < roundsPlayed - 1; i++) {
        result[i] = rounds[i];
    }
    delete[] rounds;

    return result;
}

void resetRound(Talon& talon, Player& p1, Player& p2) {
    p1.handSize = MAX_HAND_SIZE;
    p2.handSize = MAX_HAND_SIZE;

    p1.trickPoints = 0;
    p2.trickPoints = 0;

    p1.tricksWon = 0;
    p2.tricksWon = 0;
    p1.hasSurrendered = false;
    p2.hasSurrendered = false;
    p1.hasEnded = false;
    p2.hasEnded = false;
    p1.cardPlayed = { 0, 0 };
    p2.cardPlayed = { 0, 0 };
    p1.lastTrickCard = { 0, 0 };
    p2.lastTrickCard = { 0, 0 };

    p1.hand = new Card[p1.handSize];
    p2.hand = new Card[p2.handSize];

    talon.talonSize = MAX_TALON_SIZE;
    talon.talon = new Card[talon.talonSize];

    initTalon(talon);
    deal(p1, p2, talon);

    talon.trumpCard = pickTrumpSuit(talon);
}

Player gameWinner(const Player p1, const Player p2) {
    if (p1.gamePoints > p2.gamePoints) {
        return p1;
    }
    return p2;
}

void gameStart(const Settings settings) {
    Player inPlay, outOfPlay;
    inPlay.name = 1;
    outOfPlay.name = 2;
    Talon talon;

    int roundsPlayed = 1;
    Round* rounds = new Round[roundsPlayed];
    Player trick_winner;

    do {
        resetRound(talon, inPlay, outOfPlay);

        std::cout << std::endl << "Round " << roundsPlayed << " started:" << std::endl;
        do {
            inPlay = playerCommand(settings, inPlay, outOfPlay, talon, rounds, roundsPlayed);
            if (inPlay.hasEnded || inPlay.hasSurrendered) break;
            outOfPlay = playerCommand(settings, outOfPlay, inPlay, talon, rounds, roundsPlayed);
            if (outOfPlay.hasEnded || outOfPlay.hasSurrendered) break;

            trick_winner = trickWinner(settings, inPlay, outOfPlay, talon);

            if (trick_winner.name != inPlay.name) {
                outOfPlay = inPlay;
                inPlay = trick_winner;
            }
            if (!talonIsEmpty(talon) && !talon.isClosed) {
                inPlay.hand[inPlay.handSize - 1] = drawCard(talon);
                outOfPlay.hand[outOfPlay.handSize - 1] = drawCard(talon);
            }
        } while (!trickFinished(inPlay, outOfPlay));
        if (inPlay.handSize == 0 && outOfPlay.handSize == 0 && settings.lastTrickBonus) {
            inPlay.trickPoints += LAST_TRICK_BONUS;
        }
        std::cout << std::endl << "Round " << roundsPlayed << " has ended!";

        int inPlayPts = inPlay.gamePoints;
        int outOfPlayPts = outOfPlay.gamePoints;

        Player round_winner = roundWinner(inPlay, outOfPlay);

        int pointsEarned = 0;
        if (round_winner.name == 0) {
            std::cout << std::endl << "Round ended in a Tie! No points awarded." << std::endl;
        }
        if (round_winner.name == inPlay.name) {
            pointsEarned = inPlay.gamePoints - inPlayPts;
        }
        else {
            pointsEarned = outOfPlay.gamePoints - outOfPlayPts;
        }
        Round roundPlayed = { round_winner, pointsEarned, inPlay, outOfPlay };

        rounds[roundsPlayed - 1] = roundPlayed;
        rounds = increaseRounds(rounds, roundsPlayed);

        std::cout << std::endl << "Player " << round_winner.name << " has won the round and earned " << pointsEarned << " game points!" << std::endl;

        delete[] inPlay.hand;
        inPlay.hand = nullptr;

        delete[] outOfPlay.hand;
        outOfPlay.hand = nullptr;
        
        delete[] talon.talon;
        talon.talon = nullptr;

    } while (!gameFinished(settings, inPlay, outOfPlay));

    Player game_winner = gameWinner(inPlay, outOfPlay);

    std::cout << std::endl << "Game has ended." << std::endl;
    std::cout << "Player " << game_winner.name << " wins!" << std::endl;
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
    const char start[] = "start";

    do {
        std::cout << "> ";
        std::cin.getline(command, COMMAND_MAX_SIZE - 1);

        if (strCompare(command, start) == 0) {
            gameStart(settings);
        }

        const char settingsStr[] = "settings";
        if (strCompare(command, settingsStr) == 0) {
            changeSettings(settings);
        }

        const char rules[] = "rules";
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