#include "../../include/core/game.h"
#include "../../include/utils.h"
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>

int TOTAL_CARDS = 104;
int CARDS_IN_HAND = 10;

using namespace std;

Game::Game() {
    this->gameStarted = false;
    this->expectingCleanRowMove = false;
    this->board = Board();
}

Game::~Game() {
    // Destructor - Clean up players
    for (auto player : players) {
        delete player;
    }
}

void Game::addPlayer(Player* player) {
    players.emplace_back(player);
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
        Card card = Card(i, calculateBullHeard(i));
        deck.emplace_back(card);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = default_random_engine(seed);
    shuffle(begin(deck), end(deck), rng);

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
        player->setHand(hand);
    }
}

void Game::collectMoves() {
    vector<
    roundMoves.clear();

    for (auto& player : players) {
        Move* move = player->makeDecision(*this);
        roundMoves.push_back(move);
    }
}

struct PlayerMove {
    int playerIndex;
    PlaceCardMove* move;

    PlayerMove(int idx, PlaceCardMove* mv) : playerIndex(idx), move(mv) {}
};

void Game::processMoves() {
    
    int lowestCardValue = board.getLowestCardValue();
    vector<PlayerMove> playerMoves;

    // Pair each move with the corresponding player's index
    for (int i = 0; i < roundMoves.size(); i++) {
        if (auto* placeCardMove = dynamic_cast<PlaceCardMove*>(roundMoves[i])) {
            playerMoves.emplace_back(i, placeCardMove);
        }
    }

    // Sort moves by the card number
    std::sort(playerMoves.begin(), playerMoves.end(), [](const PlayerMove& a, const PlayerMove& b) {
        return a.move->getCard().getNumber() < b.move->getCard().getNumber();
    });

    int idxLowestCardPlaced = -1; // Set to -1 so in case no CleanRow move required we don't skip move in the move processing for-loop

    // If the lowest card is lower that the last card of every row, then a CleanRowMove is required
    if (playerMoves[0].move->getCard().getNumber() < lowestCardValue) {
        idxLowestCardPlaced = playerMoves[0].playerIndex;   
        setExpectingCleanRowMove(true); // Flag to let know to the engine that needs to choose a row
        Move* cleanMove = players[idxLowestCardPlaced]->makeDecision(*this); // makeDecision understands when to do a CleanRowMove
        setExpectingCleanRowMove(false);
        auto* cleanRowMove = dynamic_cast<CleanRowMove*>(cleanMove);

        if (cleanRowMove) {
            Move* move = roundMoves[idxLowestCardPlaced];
            auto* placeCardMove = dynamic_cast<PlaceCardMove*>(move);
            int score = board.cleanRow(placeCardMove->getCard(), cleanRowMove->getRowToClean());;
            // TODO: ignore score for now.
        }

        delete cleanMove; // Clean up the move after processing
    }

    // Process the rest of the moves
    for (int i = 0; i < roundMoves.size(); i++) {
        Move* move = roundMoves[i];

        if (i != idxLowestCardPlaced){ //todo bug
            if (auto* placeCardMove = dynamic_cast<PlaceCardMove*>(move)) {
                // Process PlaceCardMove
                Card card = placeCardMove->getCard();
                // Find row to add the card to
                int targetRow = board.findTargetRow(card);
                int score = board.addCardToRow(card, targetRow);
                // TODO: ignore score for now.
            }
        }

        delete move; // Clean up the move after processing
    }
    roundMoves.clear(); // Clear the vector for the next round
}

void Game::printHands(bool printBullHeads) const {
    for (auto player : players) {
        cout << player->getName()<< ": ";
        player->printHand(printBullHeads);
    }
}

bool Game::getExpectingCleanRowMove() const {
    return expectingCleanRowMove;
}

void Game::setExpectingCleanRowMove(bool expectingCleanRowMoveVal){
    expectingCleanRowMove = expectingCleanRowMoveVal;
}