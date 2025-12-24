#include <iostream>
#include <cstdlib>
#include <ctime>
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
    srand(time(nullptr));

    commandIn();

    return 0;
}