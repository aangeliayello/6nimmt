#ifndef MCTS_ENGINE_H
#define MCTS_ENGINE_H

#include "engine.h"

class MCTSEngine : public Engine {
public:
    int n_simul; // Number of simulation
    int n_init_simul; // Number of preRun simulation to find the moves to explore
    float exploration_fraction; // 

    MCTSEngine(int n_simul = 1000, int n_init_simul = 300, float exploration_fraction = 0.50);

    PlaceCardMove* makeDecisionPlaceCard(const Game& game, const std::vector<Card>& hand) override;
    CleanRowMove* makeDecisionCleanRow(const Game& game, const std::vector<Card>& hand) override;
};

#endif // MCTS_ENGINE_H