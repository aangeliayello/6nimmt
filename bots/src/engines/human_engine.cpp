#include "../../include/engines/human_engine.h"
#include "../../include/core/move.h"
#include "../../include/core/game.h"
#include "../../include/core/board.h"
#include "../../include/utils.h"
#include <iostream>

using namespace std;

HumanEngine::HumanEngine() {}

Move* HumanEngine::makeDecision(const Game& game, const std::vector<Card>& hand) {
    cout << "Boad: " << endl;
    Board board = game.getBoard();
    board.print();

    if (game.getExpectingCleanRowMove()){

    } else {
        cout << "Your hand: " << endl;
        printHand(hand, true);
        cout << endl;

        int chosenCardIndex;
        std::cout << "Choose a card to play (index): ";
        std::cin >> chosenCardIndex;

        // Create and return the move
        return new PlaceCardMove(hand[chosenCardIndex]);
    }
}