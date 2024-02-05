#ifndef RANDOM_ENGINE_H
#define RANDOM_ENGINE_H

#include "engine.h"
#include <random>

class RandomEngine : public Engine {
public:
    // Defines whether the bot is truly random or chooses the row with the least bull points in it
    bool pickMinBullsRowToClean; 
    RandomEngine(bool pickMinBullsRowToClean = false);

    PlaceCardMove* makeDecisionPlaceCard(const Game& game, const std::vector<Card>& hand) override;
    CleanRowMove* makeDecisionCleanRow(const Game& game, const std::vector<Card>& hand) override;
};

#endif // RANDOM_ENGINE_H