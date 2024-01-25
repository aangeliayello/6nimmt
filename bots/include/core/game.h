#ifndef GAME_H
#define GAME_H

#include <memory>  // For smart pointers
#include "board.h"
#include "player.h"

using namespace std;

class Game {
private:
    bool gameStarted;
    Board board;
    vector<unique_ptr<Player>> players; // Smart pointers for automatic memory management

    void distributeCards();  // Method to distribute cards to players, and initialize the board

public:
    Game();
    ~Game();
    
    const Board& getBoard() const {
        return board;
    }

    void addPlayer(Player* player);

    void startGame();

    void processRound();

};

#endif // CARD_H