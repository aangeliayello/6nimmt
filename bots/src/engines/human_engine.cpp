#include "../../include/engines/human_engine.h"
#include "../../include/core/move.h"
#include "../../include/core/game.h"
#include "../../include/core/board.h"
#include "../../include/utils.h"
#include <iostream>

using namespace std;

HumanEngine::HumanEngine() {}

PlaceCardMove* HumanEngine::makeDecisionPlaceCard(const Game& game, const std::vector<Card>& hand) {
    cout << "Board: " << endl;
    Board board = game.getBoard();
    board.print();

    cout << "Your hand: " << endl;
    printHand(hand, true);
    cout << endl;

    int chosenCardIndex;
    cout << "Choose a card to play (index): ";
    cin >> chosenCardIndex;

    // Create and return the move
    return new PlaceCardMove(hand[chosenCardIndex]);
}

CleanRowMove* HumanEngine::makeDecisionCleanRow(const Game& game, const std::vector<Card>& hand) {
    cout << "Board: " << endl;
    Board board = game.getBoard();
    board.print();
    
    cout << "Hands: " << endl;
    game.printHands();
    int chosenRow;
    cout << "Choose a row to clean (index): ";
    cin >> chosenRow;
    return new CleanRowMove(chosenRow);
}