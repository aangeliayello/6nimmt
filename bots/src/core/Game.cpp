#include "../../include/core/game.h"
#include "../../include/utils.h"
#include "../../include/globals.h"
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

Game::Game() {
    this->gameStarted = false;
    this->board = Board();
    this->waitingForInputToPlayCard = false;
    this->waitingForInputToCleanRow = false;
}

bool Game::getWaitingForInputToPlayCard() const {
    return waitingForInputToPlayCard;
}

bool Game::getWaitingForInputToCleanRow() const {
    return waitingForInputToCleanRow;
}

void Game::setBoard(Board boardToSet)  {
    board = boardToSet;
}

void Game::addPlayer(unique_ptr<Player> player) {
    players.emplace_back(std::move(player));
}

void Game::setGameStarted(bool gs){
    gameStarted = gs;
}

void Game::startGame() {
    distributeCards();
    gameStarted = true;
}

void Game::distributeCards() {
    
    // Create and shuffle a deck of cards
    vector<Card> deck;
    // Assuming Card constructor takes two parameters
    for (int i = 1; i <= TOTAL_CARDS; ++i) {
        Card card = Card(i, calculateBullHeads(i));
        deck.emplace_back(card);
    }

    shuffle(begin(deck), end(deck), rng);

    board.reset();
    // Distribute initial cards to each row on the board
    for (int r = 0; r < board.getNumberOfRows(); r++) {
        board.addCardToRow(deck.back(), r);  // Assuming addRowCard method in Board
        deck.pop_back();
    }
    // Distribute cards to players
    for (auto& player : players) {

        vector<Card> hand;
        for (int i = 0; i < CARDS_IN_HAND; i++){
            hand.push_back(deck.back());
            deck.pop_back();
        }
        std::sort(hand.begin(), hand.end(), [](const Card& a, const Card& b) {
            return a.getNumber() < b.getNumber();
        });
        
        player->setHand(hand);
    }
}

void Game::clearRoundMoves() {
    roundMoves.clear();
}
void Game::AddToRoundMoves(PlaceCardMove* move){
    roundMoves.push_back(move);
}

const vector<PlaceCardMove*> Game::getRoundMoves() const{
    return roundMoves;
}

void Game::AddCleanRowMove(CleanRowMove* newCleanRowMove){
    roundCleanRowMove = newCleanRowMove;
}

void Game::collectMoves() {
    roundMoves.clear();

    for (auto& player : players) {
        PlaceCardMove* move = player->makeDecisionPlaceCard(*this);
        roundMoves.push_back(move);
    }
}

void Game::collectMovesBots() {
    roundMoves.clear();
    int playerIdx = 0;
    for (auto& player : players) {
        if (playerIdx == players.size()-1){
            continue;
        }
        PlaceCardMove* move = player->makeDecisionPlaceCard(*this);
        roundMoves.push_back(move);
        playerIdx++;
    }

    waitingForInputToPlayCard = true;
}

// void Game::inputCardToPlay(int cardIndexToPlay) {
//     int playerIdx = players.size()-1;
//     const unique_ptr<Player>& player = players[playerIdx];
//     vector<Card>& hand = player->getHandNonConst();
//     PlaceCardMove* placeCardMove =  new PlaceCardMove(hand[cardIndexToPlay]);
//     removeCardFromHand(placeCardMove->getCard(), hand);
//     roundMoves.push_back(placeCardMove); 
//     waitingForInputToPlayCard = false;
// }

void Game::inputCardToPlay(int& cardIndexToPlay) {
    if (players.empty()) {
        throw std::runtime_error("No players in the game.");
    }

    // Directly access the last player's non-const hand.
    int playerIdx = players.size() - 1;
    auto& player = players[playerIdx]; // Non-const reference
    auto& hand = player->getHandNonConst();

    // Validate cardIndexToPlay is within the hand's bounds
    if (cardIndexToPlay < 0 || cardIndexToPlay >= hand.size()) {
        throw std::out_of_range("Card index to play is out of hand's range.");
    }

    // Create the move with smart pointer management
    auto placeCardMove = new PlaceCardMove(hand[cardIndexToPlay]);

    // Remove the card from the hand
    hand.erase(hand.begin() + cardIndexToPlay);

    // Store the move
    roundMoves.push_back(placeCardMove);
    waitingForInputToPlayCard = false;
    cardIndexToPlay = -1;
}

struct PlayerMove {
    int playerIndex;
    PlaceCardMove* move;

    PlayerMove(int idx, PlaceCardMove* mv) : playerIndex(idx), move(mv) {}
};

bool Game::checkIfInputToCleanRowNeeded() {
    int lowestCardValue = board.getLowestCardValue();
    vector<PlayerMove> playerMoves;

    // Pair each move with the corresponding player's index
    for (int i = 0; i < roundMoves.size(); i++) {
        playerMoves.emplace_back(i, roundMoves[i]);
    }

    // Sort moves by the card number
    std::sort(playerMoves.begin(), playerMoves.end(), [](const PlayerMove& a, const PlayerMove& b) {
        return a.move->getCard().getNumber() < b.move->getCard().getNumber();
    });

    int idxLowestCardPlaced = -1; // Set to -1 so in case no CleanRow move required we don't skip move in the move processing for-loop

    // If the lowest card is lower that the last card of every row, then a CleanRowMove is required
    if (playerMoves[0].move->getCard().getNumber() < lowestCardValue) {
        idxLowestCardPlaced = playerMoves[0].playerIndex;   
        if (idxLowestCardPlaced == (players.size()-1)){// Human players is last
            waitingForInputToCleanRow = true;
            return true;
        }
    }
    
    return false;
}

void Game::processMovesNoRowCleaning() {
         
    int lowestCardValue = board.getLowestCardValue();
    vector<PlayerMove> playerMoves;

    // Pair each move with the corresponding player's index
    for (int i = 0; i < roundMoves.size(); i++) {
        playerMoves.emplace_back(i, roundMoves[i]);
    }

    // Sort moves by the card number
    std::sort(playerMoves.begin(), playerMoves.end(), [](const PlayerMove& a, const PlayerMove& b) {
        return a.move->getCard().getNumber() < b.move->getCard().getNumber();
    });

    int idxLowestCardPlaced = -1; // Set to -1 so in case no CleanRow move required we don't skip move in the move processing for-loop

    // If the lowest card is lower that the last card of every row, then a CleanRowMove is required
    if (playerMoves[0].move->getCard().getNumber() < lowestCardValue) {
        idxLowestCardPlaced = playerMoves[0].playerIndex;   

        int score = board.cleanRow(roundMoves[idxLowestCardPlaced]->getCard(), 0);;
        players[idxLowestCardPlaced]->addScore(score);
    }

    // Process the rest of the moves
    for (auto playerMove : playerMoves) {
        int idx = playerMove.playerIndex;
        if (idx != idxLowestCardPlaced){
            // Process PlaceCardMove
            Card card = playerMove.move->getCard();
            // Find row to add the card to
            int targetRow = board.findTargetRow(card);
            int score = board.addCardToRow(card, targetRow);
            if (score){
                players[idx]->addScore(score);
            }
        }

        delete playerMove.move; // Clean up the move after processing
    }
    roundMoves.clear(); // Clear the vector for the next round
    waitingForInputToPlayCard = false;
    waitingForInputToCleanRow = false;
}


void Game::processMoves() {
         
    int lowestCardValue = board.getLowestCardValue();
    vector<PlayerMove> playerMoves;

    // Pair each move with the corresponding player's index
    for (int i = 0; i < roundMoves.size(); i++) {
        playerMoves.emplace_back(i, roundMoves[i]);
    }

    // Sort moves by the card number
    std::sort(playerMoves.begin(), playerMoves.end(), [](const PlayerMove& a, const PlayerMove& b) {
        return a.move->getCard().getNumber() < b.move->getCard().getNumber();
    });

    int idxLowestCardPlaced = -1; // Set to -1 so in case no CleanRow move required we don't skip move in the move processing for-loop

    // If the lowest card is lower that the last card of every row, then a CleanRowMove is required
    if (playerMoves[0].move->getCard().getNumber() < lowestCardValue) {
        idxLowestCardPlaced = playerMoves[0].playerIndex;   
        CleanRowMove* cleanRowMove = players[idxLowestCardPlaced]->makeDecisionCleanRow(*this); // makeDecision understands when to do a CleanRowMove

        if (cleanRowMove) {
            int score = board.cleanRow(roundMoves[idxLowestCardPlaced]->getCard(), cleanRowMove->getRowToClean());;
            players[idxLowestCardPlaced]->addScore(score);
        }

        delete cleanRowMove; // Clean up the move after processing
    }

    // Process the rest of the moves
    for (auto playerMove : playerMoves) {
        int idx = playerMove.playerIndex;
        if (idx != idxLowestCardPlaced){
            // Process PlaceCardMove
            Card card = playerMove.move->getCard();
            // Find row to add the card to
            int targetRow = board.findTargetRow(card);
            int score = board.addCardToRow(card, targetRow);
            if (score){
                players[idx]->addScore(score);
            }
        }

        delete playerMove.move; // Clean up the move after processing
    }
    roundMoves.clear(); // Clear the vector for the next round
}

void Game::processMovesWithCleaning(){
     
    int lowestCardValue = board.getLowestCardValue();
    vector<PlayerMove> playerMoves;

    // Pair each move with the corresponding player's index
    for (int i = 0; i < roundMoves.size(); i++) {
        playerMoves.emplace_back(i, roundMoves[i]);
    }

    // Sort moves by the card number
    std::sort(playerMoves.begin(), playerMoves.end(), [](const PlayerMove& a, const PlayerMove& b) {
        return a.move->getCard().getNumber() < b.move->getCard().getNumber();
    });

    int idxLowestCardPlaced = -1; // Set to -1 so in case no CleanRow move required we don't skip move in the move processing for-loop

    // If the lowest card is lower that the last card of every row, then a CleanRowMove is required
    if (playerMoves[0].move->getCard().getNumber() < lowestCardValue) {
        idxLowestCardPlaced = playerMoves[0].playerIndex;   
        if (roundCleanRowMove) {
            int score = board.cleanRow(roundMoves[idxLowestCardPlaced]->getCard(), roundCleanRowMove->getRowToClean());;
            players[idxLowestCardPlaced]->addScore(score);
        }
        delete roundCleanRowMove; // Clean up the move after processing
        waitingForInputToCleanRow = false;
    } else{
        throw std::runtime_error("Expecting at least one card to be lower than the minimum anchor card");
    }

    // Process the rest of the moves
    for (auto playerMove : playerMoves) {
        int idx = playerMove.playerIndex;
        if (idx != idxLowestCardPlaced){ 
            // Process PlaceCardMove
            Card card = playerMove.move->getCard();
            // Find row to add the card to
            int targetRow = board.findTargetRow(card);
            int score = board.addCardToRow(card, targetRow);
            if (score){
                players[idx]->addScore(score);
            }
        }

        delete playerMove.move; // Clean up the move after processing
    }
    roundMoves.clear(); // Clear the vector for the next round
}

void Game::printHands(bool printBullHeads) const {
    for (auto& player : players) {
        cout << player->getName()<< ": ";
        player->printHand(printBullHeads);
    }
}

void Game::printScores() const {
    for (auto& player : players) {
        cout << player->getName() << ": " << player->getScore() << endl;
    }
}

vector<int> Game::getScores() const {
    vector<int> scores;
    for (const auto& player : players) {
        scores.push_back(player->getScore());
    }
    
    return scores;
}

void Game::resetScores() const{
    for (auto& player : players){
        player->resetScore();
    }
}

string Game::getWinner() const{
    int minScore = 999999;
    string winner = "";
    int score;
    for (const auto& player : players) {
        score = player->getScore();
        if (score < minScore){
            minScore = score;
            winner = player->getName();
        } else if (score == minScore){
            winner += " & " + player->getName();
        }
    }
    return winner;
}

void Game::runGame(){
    int cards_left = players[0]->getHand().size();
    for (int i = 0; i < cards_left; i++){
        collectMoves();
        processMoves();
    }
}

void Game::resetGame() {
    gameStarted = true;
    waitingForInputToPlayCard = false;
    waitingForInputToCleanRow = false;
    resetScores();
    roundMoves.clear();
    roundCleanRowMove = nullptr;  
    distributeCards();
}