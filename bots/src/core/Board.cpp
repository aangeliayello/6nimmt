#include "../../include/core/board.h"
#include <vector>
#include <sstream>

using namespace std;

int NUMBER_OF_ROWS = 4;
int CAPACITY = 6;

Board::Board(){    
    this->capacity = CAPACITY;
    this->n_rows   = NUMBER_OF_ROWS;
    this->rows.resize(n_rows);
};

int Board::cleanRow(const Card& card, int row){
    int score = 0;
    for (const auto& c : rows[row]) {
        score += c.getBullHeads();
    }

    rows[row].clear();
    rows[row].push_back(card);

    return score;
};

int Board::addCardToRow(const Card& card, int row){
    int score = 0;

    // Added card will reach capacity
    if (rows[row].size() == (capacity - 1)){
        score = cleanRow(card, row);
    } else{
        rows[row].push_back(card);
    }

    return score;
};

int Board::getNumberOfRows() const{
    return n_rows;
};

int Board::getCapacity() const {
    return capacity;
};

int Board::getLowestCardValue() const {
    int lowestCardValue = INT_MAX;

    for (const auto& row : rows) {
        if (row[0].getNumber() < lowestCardValue){
            lowestCardValue = row[0].getNumber();
        }
    }

    return lowestCardValue;
}

int Board::findTargetRow(const Card& card) const {
    int targetRow = -1;
    int highestLastCardValue = -1;

    for (int i = 0; i < getNumberOfRows(); ++i) {
        Card lastCardInRow = rows[i].back();

        if (card.getNumber() > lastCardInRow.getNumber()) {
            // Check if this row has the highest last card value so far
            if (lastCardInRow.getNumber() > highestLastCardValue) {
                highestLastCardValue = lastCardInRow.getNumber();
                targetRow = i;
            }
        }
    }

    return targetRow; // Returns -1 if no suitable row is found
}

string Board::toString() const {
    ostringstream oss;
    for (int i = 0; i < n_rows; ++i) {
        oss << "Row " << (i + 1) << ": ";
        for (const auto& card : rows[i]) {
            oss << card.toString();
            if (!rows[i].empty()) {
                oss << ", ";
            }
        }
        oss << "\n";
    }
    return oss.str();
};

void Board::print() const {
    cout << toString();
};