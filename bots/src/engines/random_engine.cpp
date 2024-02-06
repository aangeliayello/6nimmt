#include "../../include/engines/random_engine.h"
#include "../../include/core/move.h"
#include "../../include/core/game.h"
#include "../../include/globals.h"
#include <algorithm>

using namespace std;

RandomEngine::RandomEngine(bool pickMinBullsRowToClean) {}

PlaceCardMove* RandomEngine::makeDecisionPlaceCard(const Game& game, const std::vector<Card>& hand) {
    // Randomly select a card from the hand
    int cardIndex = uniform_int_distribution<int>(0, hand.size()-1)(rng);
    // int cardIndex = 0;
    Card chosenCard = hand[cardIndex];

    // Create and return the move
    return new PlaceCardMove(chosenCard);
}

CleanRowMove* RandomEngine::makeDecisionCleanRow(const Game& game, const std::vector<Card>& hand) {
    if (true){ // Smartly choose row to clean
        vector<int> bullsPerRow = game.getBoard().getBullPerRow();
        auto minElementIterator = min_element(bullsPerRow.begin(), bullsPerRow.end());
        int chosenRow = distance(bullsPerRow.begin(), minElementIterator);
        //game.getBoard().print();
        //cout << chosenRow << endl;
        return new CleanRowMove(chosenRow);
    } else{ // Choose row to clean at random
        // Randomly select a card from the hand
        int n_rows = game.getBoard().getNumberOfRows();
        int chosenRow = uniform_int_distribution<int>(0, n_rows-1)(rng);
        return new CleanRowMove(chosenRow);
    }
}