// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "engines/engine.h"
#include "move.h"
#include <vector>
#include <string>

class Player {
protected:
    string name;
    vector<Card> hand;
    Engine* engine;

public:
    Player(string name, Engine* engine) : name(name), engine(engine) {}
    virtual ~Player() { delete engine; }

    Move* makeDecision(const Game& game) {
        return engine->makeDecision(game, hand);
    }

    string getName() const {
        return name;
    }

    void setHand(const vector<Card>& newHand) {
        hand = newHand;
    }

};

#endif // PLAYER_H