#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "../core/card.h"
#include "../core/move.h"

using namespace std;

class Game;  // Forward declaration of Game

class Engine {
public:
    virtual ~Engine() = default;
    virtual Move* makeDecision(const Game& game, const vector<Card>& hand) = 0;
};

#endif