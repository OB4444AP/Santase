#pragma once

void saveCard(std::ofstream& file, const Card& c);
void loadCard(std::ifstream& file, Card& c);
void savePlayer(std::ofstream& file, const Player& p);
void loadPlayer(std::ifstream& file, Player& p);