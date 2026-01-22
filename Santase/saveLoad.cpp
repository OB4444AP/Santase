#include <iostream>
#include <fstream>
#include "saveLoad.h"
#include "strUtils.h"

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

void saveTalon(std::ofstream& file, const Talon& talon) {
    file << talon.talonSize << " " << talon.isClosed << "\n";

    saveCard(file, talon.trumpCard);

    file << talon.lastTrickWinner.name << "\n";

    for (int i = 0; i < MAX_TALON_SIZE; i++) {
        saveCard(file, talon.talon[i]);
    }
}

void loadTalon(std::ifstream& file, Talon& talon) {
    file >> talon.talonSize >> talon.isClosed;
    loadCard(file, talon.trumpCard);

    int lastWinnerName;
    file >> lastWinnerName;
    talon.lastTrickWinner.name = lastWinnerName;

    if (talon.talon != nullptr) {
        delete[] talon.talon;
    }
    talon.talon = new Card[MAX_TALON_SIZE];

    for (int i = 0; i < MAX_TALON_SIZE; i++) {
        loadCard(file, talon.talon[i]);
    }
}

void saveSettings(std::ofstream& file, const Settings& s) {
    file << s.pointsToWin << " " << s.marriagePoints_nonTrump << " "
        << s.marriagePoints_trump << " " << s.showPlayerPoints << " "
        << s.lastTrickBonus << "\n";
}

void loadSettings(std::ifstream& file, Settings& s) {
    file >> s.pointsToWin >> s.marriagePoints_nonTrump
        >> s.marriagePoints_trump >> s.showPlayerPoints
        >> s.lastTrickBonus;
}

void saveGame(const char* filename, const Player& p1, const Player& p2, const Talon& talon, const Settings& settings, const Round* rounds, int roundsPlayed) {
    char fullFilename[100];
    fullFilename[0] = '\0';

    strAppend(fullFilename, filename);
    strAppend(fullFilename, ".txt");

    std::ofstream file(fullFilename);

    if (!file.is_open()) {
        std::cout << "error";
        return;
    }

    saveSettings(file, settings);
    saveTalon(file, talon);
    savePlayer(file, p1);
    savePlayer(file, p2);

    file << roundsPlayed << "\n";
    for (int i = 0; i < roundsPlayed - 1; i++) {
        file << rounds[i].pointsWon << "\n";
        file << rounds[i].winner.name << " "
            << rounds[i].p1.name << " " << rounds[i].p1.trickPoints << " "
            << rounds[i].p2.name << " " << rounds[i].p2.trickPoints << "\n";
    }

    file.close();
    std::cout << "Game saved successfully as '" << fullFilename << "'." << std::endl;
}

bool loadGame(const char* filename, Player& p1, Player& p2, Talon& talon, Settings& settings, Round*& rounds, int& roundsPlayed) {
    char fullFilename[100];
    fullFilename[0] = '\0';

    strAppend(fullFilename, filename);
    strAppend(fullFilename, ".txt");

    std::ifstream file(fullFilename);

    if (!file.is_open()) {
        std::cout << "file not found " << fullFilename << std::endl;
        return false;
    }

    loadSettings(file, settings);
    loadTalon(file, talon);
    loadPlayer(file, p1);
    loadPlayer(file, p2);

    file >> roundsPlayed;

    if (rounds != nullptr) delete[] rounds;

    rounds = new Round[roundsPlayed];

    for (int i = 0; i < roundsPlayed - 1; i++) {
        file >> rounds[i].pointsWon;
        file >> rounds[i].winner.name
            >> rounds[i].p1.name >> rounds[i].p1.trickPoints
            >> rounds[i].p2.name >> rounds[i].p2.trickPoints;

        rounds[i].winner.hand = nullptr;
        rounds[i].p1.hand = nullptr;
        rounds[i].p2.hand = nullptr;
    }

    file.close();
    std::cout << "Game loaded successfully from '" << fullFilename << "'." << std::endl;
    return true;
}