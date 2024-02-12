// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "../engines/engine.h"
#include "move.h"
#include <vector>
#include <string>

class Game;  // Forward declaration of Game

class Player {
protected:
    string name;
    int score;
    vector<Card> hand;
    Engine* engine;

public:
    Player(string name, Engine* engine);
    virtual ~Player();

    PlaceCardMove* makeDecisionPlaceCard(const Game& game);
    
    CleanRowMove* makeDecisionCleanRow(const Game& game);

    int addScore(int incrementScore); 

    int getScore() const;

    string getName() const;

    void setHand(const vector<Card>& newHand);

    const vector<Card>& getHand() const;

    vector<Card>& getHandNonConst(); // Non-const version

    void printHand(bool printBullHeads = false) const ;
};

#endif // PLAYER_H