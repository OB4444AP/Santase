#pragma once

const int MAX_HAND_SIZE = 6;
const int MAX_TALON_SIZE = 24;

struct Card {
    int suit;
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