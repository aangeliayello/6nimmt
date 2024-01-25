#ifndef MOVE_H
#define MOVE_H

#include "card.h"

class Move {
public:
    virtual ~Move() = default;
};

class PlaceCardMove : public Move {
private:
    Card card;

public:
    PlaceCardMove(const Card& card) : card(card) {}
    
    Card getCard();
};

class CleanRowMove : public Move {
private:
    int rowToClean;

public:
    CleanRowMove(const Card& card, int row) : rowToClean(row) {}
    
    int getRowToClean();
};

#endif // MOVE_H