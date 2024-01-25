#ifndef CARD_H
#define CARD_H

#include <string>
#include <iostream>

class Card {
private:
    int number;
    int bullHeads;

public:
    Card(int number, int bullHeads);

    int getNumber() const;
    int getBullHeads() const;

    string toString() const;
    void print() const;
};

#endif // CARD_H