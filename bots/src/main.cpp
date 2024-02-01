#include "../include/engines/engine.h"
#include "../include/engines/random_engine.h"
#include "../include/engines/human_engine.h"
#include "../include/engines/mc_engine.h"
#include "../include/core/game.h"
#include "../include/core/player.h"
#include "../include/core/card.h"
#include "../include/globals.h"

std::mt19937 rng; 
#include <iostream>

int main() {
    // Set Random seed
    vector<vector<int>> scoresVector;
    vector<int> wins(4, 0);

    for (int k = 0; k < 1000; k++){
        rng.seed(k);

        Game game = Game();    
        // Add players
        // For simplicity, adding two human and two AI players
        game.addPlayer(new Player("mc", new MCEngine(10000)));
        game.addPlayer(new Player("random", new RandomEngine(true)));
        game.addPlayer(new Player("random", new RandomEngine(true)));
        game.addPlayer(new Player("random", new RandomEngine(true)));

        // Start the game
        game.startGame();
        for (int i = 0; i< 10; i++){
            cout << "**** Round " << i << " **************";
            cout << "**** Board *****" << endl;
            Board board = game.getBoard();
            board.print();
            cout << "**** Scores *****" << endl;
            game.printScores();
            cout << "**** Hand *****" << endl;
            game.printHands(true);
            cout << "**** Collection Moves *****" << endl;
            game.collectMoves();
            game.processMoves();
        }

        cout << "**** GAME OVER **************";
        cout << "**** Board *****" << endl;
        Board board = game.getBoard();
        board.print();
        cout << "**** Scores *****" << endl;
        game.printScores();
        scoresVector.push_back(game.getScores());

        int currMin = 9999;
        int idxCurrMin;
        for (int l = 0; l < scoresVector[0].size(); l++){
            if (scoresVector.back()[l] < currMin){
                currMin = scoresVector.back()[l];
                idxCurrMin = l;
            }
        }
        wins[idxCurrMin] += 1;
        cout << "Wins: " << endl;
        for (int w: wins){
            cout << w << ", ";
        }
        cout << endl;

        cout << "Score: " << endl;
        for (auto v :scoresVector){
            for (int i: v){
                cout << i << ", ";
            }
            cout << endl;
        }
    }
}


