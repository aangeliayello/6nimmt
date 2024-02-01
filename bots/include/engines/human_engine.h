#ifndef HUMAN_ENGINE_H
#define HUMAN_ENGINE_H

#include "engine.h"

class HumanEngine : public Engine {
public:

    HumanEngine();

    Move* makeDecision(const Game& game, const std::vector<Card>& hand) override;
};

#endif // HUMAN_ENGINE_H