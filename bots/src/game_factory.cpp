
#include "../include/game_factory.h"
#include "../include/engines/engine.h"
#include "../include/engines/random_engine.h"
#include "../include/engines/human_engine.h"
#include "../include/engines/mc_engine.h"
#include "../include/engines/mcts_engine.h"
#include "../include/globals.h"
#include <chrono>

std::mt19937 rng; 

void startGame(Game& game) {
    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

    // Add players
    // For simplicity, adding two human and two AI players
    int n_simul = 10*10000;
    // game.addPlayer(new Player("mc", new MCTSEngine(n_simul, 0.3*n_simul, 0.5)));
    // game.addPlayer(make_unique<Player>("random", new RandomEngine(true)));
    // game.addPlayer(make_unique<Player>("random", new RandomEngine(true)));
    // game.addPlayer(make_unique<Player>("random", new RandomEngine(true)));
    game.addPlayer(make_unique<Player>("MC", new MCEngine(n_simul)));
    game.addPlayer(make_unique<Player>("alo", new HumanEngine()));

    // Start the game
    game.startGame();
    game.printHands();
    return;
}

vector<vector<int>> getBoard(const Game& game){

    vector<vector<Card>> board = game.getBoard().getRows();
    vector<vector<int>> boardUi(board.size());

    int rowIdx = 0;
    for (auto& row : board){
        for (auto& card : row){
           boardUi[rowIdx].push_back(card.getNumber());
        }
        rowIdx++;
    }

    return boardUi;
}

vector<int> getHand(const Game& game) {

    // TODO: 
    // std::transform(hand.begin(), hand.end(), back_inserter(handUi), [](const Card& card) {
    //     return card.getNumber();
    // });
    const vector<unique_ptr<Player>>& players = game.getPlayers(); // Assuming getPlayers returns a reference
    const Player* humanPlayer = players.back().get();
    const vector<Card>& hand = humanPlayer->getHand();

    vector<int> handUi(hand.size());

    int rowIdx = 0;
    for (auto& card : hand){
        handUi[rowIdx] = card.getNumber();
        rowIdx++;
    }
    return handUi;
}

vector<int> getShownCards(const Game& game){
    vector<int> cardsToShow;
    vector<PlaceCardMove*> roundMoves = game.getRoundMoves();
    for (auto& move : roundMoves){
        cardsToShow.push_back(move->getCard().getNumber());
    }

    return cardsToShow;
}