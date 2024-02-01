#include "../include/engines/engine.h"
#include "../include/engines/random_engine.h"
#include "../include/engines/human_engine.h"
#include "../include/engines/mc_engine.h"
#include "../include/core/game.h"
#include "../include/core/player.h"
#include "../include/core/card.h"
#include "../include/globals.h"
#include <chrono>
#include <iostream>

std::mt19937 rng; 

int main() {
    std::cout << "Elapsed time: " << endl;

    // Set Random seed
    vector<vector<int>> scoresVector;
    vector<int> wins(4, 0);
    int K = 100;
    int n_simul = 100;
    auto start = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < K; k++){
        rng.seed(k);

        Game game = Game();    
        // Add players
        // For simplicity, adding two human and two AI players
        game.addPlayer(new Player("mc", new MCEngine(n_simul)));
        game.addPlayer(new Player("random", new RandomEngine(true)));
        game.addPlayer(new Player("random", new RandomEngine(true)));
        game.addPlayer(new Player("random", new RandomEngine(true)));

        // Start the game
        game.startGame();
        for (int i = 0; i< 10; i++){
            //1cout << "**** Round " << i << " **************";
            //1cout << "**** Board *****" << endl;
            Board board = game.getBoard();
            //1board.print();
            //1cout << "**** Scores *****" << endl;
            //1game.printScores();
            //1cout << "**** Hand *****" << endl;
            //1game.printHands(true);
            //1cout << "**** Collection Moves *****" << endl;
            game.collectMoves();
            game.processMoves();
        }

        //1cout << "**** GAME OVER **************";
        //1cout << "**** Board *****" << endl;
        Board board = game.getBoard();
        //1board.print();
        //1cout << "**** Scores *****" << endl;
        //1game.printScores();
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
        //1cout << "Wins: " << endl;
        //1for (int w: wins){
        //1    cout << w << ", ";
        //1}
        //1cout << endl;

        //1cout << "Score: " << endl;
        // for (auto v :scoresVector){
        //     for (int i: v){
        //         cout << i << ", ";
        //     }
        //     cout << endl;
        // }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << "K:" << K << ", NS: " << n_simul << std::endl;
}


