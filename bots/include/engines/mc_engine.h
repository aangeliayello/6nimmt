#ifndef MC_ENGINE_H
#define MC_ENGINE_H

#include "engine.h"

class MCEngine : public Engine {
public:
    int n_simul;

    MCEngine(int n_simul = 1000);

    Move* makeDecision(const Game& game, const std::vector<Card>& hand) override;
};

#endif // MC_ENGINE_H