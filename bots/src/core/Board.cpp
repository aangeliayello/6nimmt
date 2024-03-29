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
    this->cardsCleaned = {};
};
 void Board::reset(){
    rows.clear();
    rows.resize(n_rows);
    cardsCleaned = {};
 }

int Board::cleanRow(const Card& card, int row){
    int score = 0;
    for (const auto& c : rows[row]) {
        score += c.getBullHeads();
    }
    cardsCleaned.insert(cardsCleaned.end(), rows[row].begin(), rows[row].end());
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

vector<Card> Board::getCardsCleaned() const {
    return cardsCleaned;
};

int Board::getLowestCardValue() const {
    int lowestCardValue = INT_MAX;

    for (const auto& row : rows) {
        if (row.back().getNumber() < lowestCardValue){
            lowestCardValue = row.back().getNumber();
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

vector<int> Board::getBullPerRow() const {
    vector<int> bullsPerRow(n_rows, 0);
    for (int i = 0; i < n_rows; i++){
        for (auto& c : rows[i]){
            bullsPerRow[i] += c.getBullHeads();
        }
    }
    return bullsPerRow;
}

vector<vector<Card>> Board::getRows() const {
    return rows;
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