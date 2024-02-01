#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "core/card.h"

using namespace std;

int calculateBullHeads(int cardValue);

void printHand(vector<Card> hand, bool printBullHeads);

void removeCardFromHand(Card card, vector<Card>& hand);

#endif