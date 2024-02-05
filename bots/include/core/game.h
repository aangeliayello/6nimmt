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
    vector<Player*> players; // Smart pointers for automatic memory management
    vector<PlaceCardMove*> roundMoves; // Stores moves for the round
    void distributeCards();  // Method to distribute cards to players, and initialize the board

public:
    Game();
    ~Game();
    
    const Board& getBoard() const {
        return board;
    }

    const vector<Player*>& getPlayers() const {
        return players;
    }
    
    void setBoard(Board boardToSet);

    void addPlayer(Player* player);

    void setGameStarted(bool gs);

    void startGame();
    
    void clearRoundMoves();

    void AddToRoundMoves(PlaceCardMove* move);

    void collectMoves();

    void processMoves();

    void printHands(bool printBullHeads = false) const;

    void printScores() const;

    vector<int> getScores() const;

    void runGame();
};

#endif // GAME_H