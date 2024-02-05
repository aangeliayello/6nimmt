#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "core/card.h"
#include "core/game.h"
#include <unordered_set>

using namespace std;

int calculateBullHeads(int cardValue);

void printHand(vector<Card> hand, bool printBullHeads);

void removeCardFromHand(Card card, vector<Card>& hand);

vector<float> matrixAverage(vector<vector<int>> values);

unordered_set<int> getSeenCards(const Game& game, const vector<Card>& hand);

float scoresToLoss(vector<int> scores);

#endif