#include "board.h"
#include <vector>

using namespace std;

int NUMBER_OF_ROWS = 4;
int CAPACITY = 6;

Board::Board(){
    vector<vector<Cards>> rows;
    this->capacity = CAPACITY;
    this->n_rows = NUMBER_OF_ROWS;
    this->rows     = rows.resize(n_rows);
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

int Board::getNumberOfRows(){
    return n_rows;
};

int Board::getCapacity() const {
    return capacity;
};

std::string Board::toString() const {
    std::ostringstream oss;
    for (int i = 0; i < n_rows; ++i) {
        oss << "Row " << (i + 1) << ": ";
        for (const auto& card : rows[i]) {
            oss << card.toString();
            if (!rows[i].empty()) {
                oss <<  << ", "
            }
        }
        oss << "\n";
    }
    return oss.str();
};

void Board::print() const {
    std::cout << toString();
};