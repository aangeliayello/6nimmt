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