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

int main(int argc, char* argv[]) {
    rng.seed(0);

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " K n_simul" << std::endl;
        return 1;
    }

    int K = std::atoi(argv[1]);       // Convert the first argument to an integer (K)
    int n_simul = std::atoi(argv[2]); // Convert the second argument to an integer (n_simul)

    std::cout << "K: " << K << std::endl;
    std::cout << "n_simul: " << n_simul << std::endl;
    // Set Random seed
    vector<vector<int>> scoresVector;
    vector<int> wins(4, 0);
    auto start = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < K; k++){
        Game game = Game();    
        // Add players
        // For simplicity, adding two human and two AI players
        //game.addPlayer(new Player("mc", new MCEngine(n_simul)));
        game.addPlayer(new Player("mc", new MCTSEngine(n_simul, 0.3*n_simul, 0.5)));
        game.addPlayer(new Player("filip", new HumanEngine()));

        // Start the game
        // cout << k << ", " << rng << endl;
        game.startGame();
        // cout << k << ", " << rng << endl;
        game.printHands();
        game.getBoard().print();
        for (int i = 0; i< 10; i++){
            // cout << k << "-" << i << ", " << rng << endl;

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
    }

    cout << "Wins: " << endl;
    for (int w: wins){
        cout << w << ", ";
    }
    cout << endl;

    cout << "Score: " << endl;
    vector<float> avgScores = matrixAverage(scoresVector);
    for (auto v :avgScores){
        cout << v << ", ";
    }
    cout << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << "K:" << K << ", NS: " << n_simul << std::endl;
}


