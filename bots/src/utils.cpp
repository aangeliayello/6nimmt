#include "../include/utils.h"

int calculateBullHeard(int cardValue){
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
        std::cout << card.getNumber();
        if (printBullHeads) {
            std::cout << " " << card.getBullHeads() << "b";
        }
        std::cout << ", ";
    }
    cout << endl;
}
