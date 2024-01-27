#include "../../include/engines/random_engine.h"
#include "../../include/core/move.h"
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()

using namespace std;

RandomEngine::RandomEngine() {
    // Initialize random seed
    srand(std::time(nullptr));
}

Move* RandomEngine::makeDecision(const Game& game, const std::vector<Card>& hand) {
    // Randomly select a card from the hand
    int cardIndex = rand() % hand.size();
    Card chosenCard = hand[cardIndex];

    // Create and return the move
    return new PlaceCardMove(chosenCard);
}