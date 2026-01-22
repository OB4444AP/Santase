#include <iostream>
#include <fstream>
#include "saveLoad.h"
#include "strUtils.cpp"
#include "Santase.cpp"

void saveCard(std::ofstream& file, const Card& c) {
    file << c.suit << " " << c.value << "\n";
}

void loadCard(std::ifstream& file, Card& c) {
    file >> c.suit >> c.value;
}

void savePlayer(std::ofstream& file, const Player& p) {
    file << p.name << " " << p.trickPoints << " " << p.gamePoints << " "
        << p.hasSurrendered << " " << p.hasSurrenderedForever << " " << p.hasEnded << " "
        << p.tricksWon << " " << p.roundsWon << "\n";

    saveCard(file, p.cardPlayed);
    saveCard(file, p.lastTrickCard);

    file << p.handSize << "\n";
    for (int i = 0; i < p.handSize; i++) {
        saveCard(file, p.hand[i]);
    }
}

void loadPlayer(std::ifstream& file, Player& p) {
    file >> p.name >> p.trickPoints >> p.gamePoints
        >> p.hasSurrendered >> p.hasSurrenderedForever >> p.hasEnded
        >> p.tricksWon >> p.roundsWon;

    loadCard(file, p.cardPlayed);
    loadCard(file, p.lastTrickCard);

    int loadedHandSize;
    file >> loadedHandSize;

    if (p.hand != nullptr) {
        delete[] p.hand;
    }

    p.hand = new Card[MAX_HAND_SIZE];
    p.handSize = loadedHandSize;

    for (int i = 0; i < p.handSize; i++) {
        loadCard(file, p.hand[i]);
    }
}