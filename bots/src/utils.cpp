#include "../include/utils.h"
#include <algorithm>

using namespace std;

int calculateBullHeads(int cardValue){
    if (cardValue == 55){
        return 7;
    } else if (cardValue % 11 == 0){
        return 5;
    } else if (cardValue % 10 == 0){
        return 3;
    } else if (cardValue % 5 == 0){
        return 2;
    } else{
        return 1;
    }
}


void printHand(vector<Card> hand, bool printBullHeads){
    for (const auto& card : hand) {
        cout << card.getNumber();
        if (printBullHeads) {
            cout << " " << card.getBullHeads() << "b";
        }
        cout << ", ";
    }
    cout << endl;
}

void removeCardFromHand(Card card, vector<Card>& hand) {
    int cardValue = card.getNumber();
    auto it = find_if(hand.begin(), hand.end(), 
        [cardValue](Card c) { return c.getNumber() == cardValue; });
    if (it != hand.end()) {
        hand.erase(it);
    }
}

vector<float> matrixAverage(vector<vector<int>> values){
    vector<float> avg(values[0].size());
    for(int c = 0; c < avg.size(); c++){
        for (int r = 0; r < values.size(); r++){
            avg[c] += float(values[r][c])/values.size();
        }
    }
    return avg;
}

unordered_set<int> getSeenCards(const Game& game, const vector<Card>& hand){
    unordered_set<int> seenCards = {};
    // Add cards cleaned
    Board board = game.getBoard();
    for (auto& c : board.getCardsCleaned()){
        seenCards.insert(c.getNumber());
    }

    // Add cards in the board
    for (auto& row : board.getRows()){
        for (auto& c : row){
            seenCards.insert(c.getNumber());
        }
    }

    // Add cards in the player's hand
    for (auto& c : hand){
        seenCards.insert(c.getNumber());
    }

    return seenCards;
};

float scoresToLoss(vector<int> scores){
    // Assuming 0th position is our score

    float loss = scores[0];
    for (int i = 1; i < scores.size(); i++){
        loss += -scores[i];
    }

    return loss/scores.size();
}