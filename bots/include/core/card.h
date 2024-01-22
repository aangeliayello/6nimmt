#ifndef CARD_H
#define CARD_H

class Card {
private:
    int number;
    int bullHeads;

public:
    Card(int number, int bullHeads);

    int getNumber() const;
    int getBullHeads() const;
};

#endif // CARD_H