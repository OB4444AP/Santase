#pragma once
#include <fstream>
#include "Structs.h"

void saveCard(std::ofstream& file, const Card& c);
void loadCard(std::ifstream& file, Card& c);
void savePlayer(std::ofstream& file, const Player& p);
void loadPlayer(std::ifstream& file, Player& p);
void saveTalon(std::ofstream& file, const Talon& talon);
void loadTalon(std::ifstream& file, Talon& talon);
void saveSettings(std::ofstream& file, const Settings& s);
void loadSettings(std::ifstream& file, Settings& s);
void saveGame(const char* filename, const Player& p1, const Player& p2, const Talon& talon, const Settings& settings, const Round* rounds, int roundsPlayed);
bool loadGame(const char* filename, Player& p1, Player& p2, Talon& talon, Settings& settings, Round*& rounds, int& roundsPlayed);