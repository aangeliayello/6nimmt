#include "../include/engines/engine.h"
#include "../include/engines/random_engine.h"
#include "../include/engines/human_engine.h"
#include "../include/core/game.h"
#include "../include/core/player.h"
#include "../include/core/card.h"
#include <iostream>

int main() {
    Card c = Card(1, 3);
    c.print();

    Game game = Game();    
    // Add players
    // For simplicity, adding two human and two AI players
    game.addPlayer(new Player("human", new HumanEngine()));
    game.addPlayer(new Player("random", new RandomEngine()));

    // Start the game
    game.startGame();
    game.printHands();
    game.printHands(true);

    
    game.collectMoves();
    game.processMoves();
    Board board = game.getBoard();
    board.print();
}


