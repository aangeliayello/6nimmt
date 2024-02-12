#ifndef GAME_H
#define GAME_H

#include <memory>  // For smart pointers
#include "board.h"
#include "player.h"

using namespace std;

class Game {
private:
    bool gameStarted;
    bool waitingForInputToPlayCard;
    bool waitingForInputToCleanRow;
    Board board;
    vector<unique_ptr<Player>> players; // Smart pointers for automatic memory management
    vector<PlaceCardMove*> roundMoves; // Stores moves for the round
    CleanRowMove* roundCleanRowMove;

public:
    Game();
    ~Game() = default;
    
    void distributeCards();  // Method to distribute cards to players, and initialize the board
    
    bool getWaitingForInputToPlayCard() const;

    bool getWaitingForInputToCleanRow() const;

    const Board& getBoard() const {
        return board;
    }

    const vector<unique_ptr<Player>>& getPlayers() const {
        return players;
    }

    vector<unique_ptr<Player>>& getPlayersV2() {
        return players;
    }

    int getNumberOfPlayers() const {
        return players.size();
    }
    
    void setBoard(Board boardToSet);

    void addPlayer(unique_ptr<Player> player);

    void setGameStarted(bool gs);

    void startGame();
    
    void clearRoundMoves();

    void AddToRoundMoves(PlaceCardMove* move);

    const vector<PlaceCardMove*> getRoundMoves() const;

    void AddCleanRowMove(CleanRowMove* move);

    void collectMoves();

    void collectMovesBots();

    void inputCardToPlay(int& cardIndexToPlay);
    
    bool checkIfInputToCleanRowNeeded();

    void processMovesNoRowCleaning();

    void processMoves();

    void processMovesWithCleaning();

    void printHands(bool printBullHeads = false) const;

    void printScores() const;

    vector<int> getScores() const;

    void runGame();
};

#endif // GAME_H