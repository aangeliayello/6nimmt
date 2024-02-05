#ifndef HUMAN_ENGINE_H
#define HUMAN_ENGINE_H

#include "engine.h"

class HumanEngine : public Engine {
public:

    HumanEngine();

    PlaceCardMove* makeDecisionPlaceCard(const Game& game, const std::vector<Card>& hand) override;
    CleanRowMove* makeDecisionCleanRow(const Game& game, const std::vector<Card>& hand) override;
};

#endif // HUMAN_ENGINE_H