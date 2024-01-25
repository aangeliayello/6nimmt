#include "game.h"
#include "utils.h"

int TOTAL_CARDS = 104;

Game::Game() : board(n_rows, capacity) {
    vector<Card> rows = [];
    vector<Player> players = [];

    this->gameStarted = false;
    this->board = new Board(rows);
    this->layers = playes;
}

void Game::addPlayer(Player* player) {
    players.emplace_back(player);
}

void Game::start() {
    distributeCards();
    gameStarted = true;
}

void Game::distributeCards() {
    
    // Create and shuffle a deck of cards
    std::vector<Card> deck;
    // Assuming Card constructor takes two parameters
    for (int i = 1; i <= TOTAL_CARDS; ++i) {
        Card card = new Card(i, calculateBullHeard(i))
        deck.emplace_back(/* parameters for Card */);
    }

    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(deck), std::end(deck), rng);

    // Distribute initial cards to each row on the board
    for (int i = 0; i < board.getNumberOfRows(); ++i) {
        board.addRowCard(deck.back());  // Assuming addRowCard method in Board
        deck.pop_back();
    }
    // Distribute cards to players
    for (auto& player : players) {
        std::vector<Card> hand = /* logic to create a hand */;
        player->setHand(hand);
    }
}


void Game::GetMoves() {
    for (auto& player : players) {
        std::unique_ptr<Move> move = player->makeDecision(*this);
        applyMove(*move);
    }

    // TODO: Check if subround needed. 
}
