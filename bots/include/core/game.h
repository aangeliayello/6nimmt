#ifndef GAME_H
#define GAME_H

#include <memory>  // For smart pointers
#include "board.h"
#include "player.h"

using namespace std;

class Game {
private:
    bool gameStarted;
    bool expectingCleanRowMove;
    Board board;
    vector<Player*> players; // Smart pointers for automatic memory management
    vector<Move*> roundMoves; // Stores moves for the round

    void distributeCards();  // Method to distribute cards to players, and initialize the board

public:
    Game();
    ~Game();
    
    const Board& getBoard() const {
        return board;
    }

    void addPlayer(Player* player);

    void startGame();

    void collectMoves();

    void processMoves();

    void printHands(bool printBullHeads = false) const;

    bool getExpectingCleanRowMove() const;

    void setExpectingCleanRowMove(bool expectingCleanRowMoveVal);

};

#endif // CARD_H