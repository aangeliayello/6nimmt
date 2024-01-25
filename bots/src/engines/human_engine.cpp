#include "human_engine.h"
#include "move.h"  // Assuming this includes PlaceCardMove and CleanRowMove
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time()
#include <iostream>

using namespace std;

HumaneEngine::HumaneEngine() {}

Move* HumaneEngine::makeDecision(const Game& game, const std::vector<Card>& hand) {
    cout << "Boad: " << endl;
    Board board = game.getBoard();

    cout << "Your hand: " << endl;
    for (const auto& c : hand) {        
        cout << c.toString() << ', ';
    }
    cout << endl << endl;

    int chosenCardIndex;
    std::cout << "Choose a card to play (index): ";
    std::cin >> chosenCardIndex;

    // Create and return the move
    return new PlaceCardMove(hand[chosenCardIndex]);
}