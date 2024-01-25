
#include "card.h"

using namespace std;

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

string Card::toString() const {
    string s =  to_string(number) + " [" + to_string(bullHeads) + "]";
    return s;
}

void Card::print() const {
    cout << toString() << endl;
    return;
}