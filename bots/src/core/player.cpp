#include "../../include/core/player.h"
#include <string>
#include "../../include/engines/engine.h"

using namespace std;

class Game;  // Forward declaration of Game

Player::Player(string name, Engine* engine) : name(name), engine(engine), score(0) {}

Player::~Player() { 
    delete engine; 
}

Move* Player::makeDecision(const Game& game) {
    return engine->makeDecision(game, hand);
}

string Player::getName() const {
    return name;
}

void Player::setHand(const vector<Card>& newHand) {
    hand = newHand;
}

int Player::addScore(int incrementScore) {
    score += incrementScore;
    return score;
}

int Player::getScore() const {
    return score;
}

void Player::printHand(bool printBullHeads) const {
    for (const auto& card : hand) {
        std::cout << card.getNumber();
        if (printBullHeads) {
            std::cout << " " << card.getBullHeads() << "b";
        }
        std::cout << ", ";
    }
    cout << endl;
}