#include "../../include/core/player.h"
#include "../../include/engines/engine.h"
#include "../../include/utils.h"
#include <string>
#include <algorithm>

using namespace std;

class Game;  // Forward declaration of Game

Player::Player(string name, Engine* engine) : name(name), engine(engine), score(0) {}

Player::~Player() { 
    //delete engine; 
}

PlaceCardMove* Player::makeDecisionPlaceCard(const Game& game) {
    PlaceCardMove* placeCardMove = engine->makeDecisionPlaceCard(game, hand);
    // Remove car from hand
    removeCardFromHand(placeCardMove->getCard(), hand);
    
    return placeCardMove;
}

CleanRowMove* Player::makeDecisionCleanRow(const Game& game) {
    return engine->makeDecisionCleanRow(game, hand);
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

int Player::resetScore() {
    score = 0;
    return score;
}

int Player::getScore() const {
    return score;
}

const vector<Card>& Player::getHand() const {
    return hand;
}

vector<Card>& Player::getHandNonConst() {
    return hand;
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