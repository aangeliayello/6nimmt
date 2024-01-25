#ifndef ENGINE_H
#define ENGINE_H

#include "game.h" // Assuming Game class is defined
#include "move.h" // Assuming Move class is defined

using namespace std;

class Engine {
public:
    virtual ~Engine() = default;
    virtual Move* makeDecision(const Game& game, const vector<Card>& hand) = 0;
};

#endif