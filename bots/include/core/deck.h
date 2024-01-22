#ifndef DECK_H  
#define DECK_H

#include "card.h"
#include <vector>

using namespace std;

class Deck {
private:
    vector<Card> cards;

public: 
    Deck();

    Card draw();
};
#endif // DECK_H