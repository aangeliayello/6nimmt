#include "../include/engines/engine.h"
#include "../include/engines/random_engine.h"
#include "../include/engines/human_engine.h"
#include "../include/engines/mc_engine.h"
#include "../include/engines/mcts_engine.h"
#include "../include/core/game.h"
#include "../include/core/player.h"
#include "../include/core/card.h"
#include "../include/globals.h"
#include "../include/utils.h"
#include <chrono>
#include <iostream>

std::mt19937 rng; 

Game startGame(int argc, char* argv[]) {
    rng.seed(123);

    Game game = Game();    
    // Add players
    // For simplicity, adding two human and two AI players
    game.addPlayer(new Player("alo", new HumanEngine()));
    // game.addPlayer(new Player("mc", new MCTSEngine(n_simul, 0.3*n_simul, 0.5)));
    game.addPlayer(new Player("random", new RandomEngine(true)));
    game.addPlayer(new Player("random", new RandomEngine(true)));
    game.addPlayer(new Player("random", new RandomEngine(true)));

    // Start the game
    game.startGame();
    game.printHands();
    for (int i = 0; i< 10; i++){
        game.collectMoves();
        game.processMoves();
    }
}


#include "GameFactory.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Card Game");
    GameFactory gameFactory;

    // Start a new game with default settings
    GameSettings settings; // Define settings as needed
    gameFactory.startGame(settings);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Handle other events like mouse clicks
            if (event.type == sf::Event::MouseButtonPressed) {
                // Determine action based on mouse click
                PlayerAction action; // Fill in action details
                gameFactory.processTurn(action);
            }
        }

        // Get the current state of the game
        GameState gameState = gameFactory.getGameState();

        // Update the UI based on the current game state
        // ...

        window.clear();
        // Draw the UI elements
        window.display();
    }

    return 0;
}