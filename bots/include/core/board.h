#ifndef BOARD_H
#define BOARD_H

#include "card.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Board {
private:
    vector<vector<Card>> rows;
    vector<Card> cardsCleaned;
    int capacity;
    int n_rows;

public:
    Board(); 

    int addCardToRow(const Card& card, int row);

    int cleanRow(const Card& card, int row);

    int getNumberOfRows() const;

    int getCapacity() const;

    vector<Card> getCardsCleaned() const;

    int getLowestCardValue() const;

    int findTargetRow(const Card& card) const;

    vector<int> getBullPerRow() const; 

    vector<vector<Card>> getRows() const; 

    string toString() const;

    void print() const;

};

#endif // BOARD_H