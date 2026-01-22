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