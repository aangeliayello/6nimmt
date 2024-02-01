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
     
    Card getCard() const;
};

class CleanRowMove : public Move {
private:
    int rowToClean;

public:
    CleanRowMove(int row) : rowToClean(row) {}
    
    int getRowToClean() const;
};

#endif // MOVE_H