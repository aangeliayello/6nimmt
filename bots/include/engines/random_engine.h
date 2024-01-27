#ifndef RANDOM_ENGINE_H
#define RANDOM_ENGINE_H

#include "engine.h"
#include <random>

class RandomEngine : public Engine {
public:
    RandomEngine();

    Move* makeDecision(const Game& game, const std::vector<Card>& hand) override;
};

#endif // RANDOM_ENGINE_H