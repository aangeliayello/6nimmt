#include "card.h"

Card::Card(int number, int bullHeads) {
    this->number     = number;
    this->bullHeads  = bullHeads;
}

int Card::getNumber() const {
    return number;
}

int Card::getBullHeads() const {
    return bullHeads;
}