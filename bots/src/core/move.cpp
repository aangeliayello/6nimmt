
#include "../../include/core/move.h"

using namespace std;

Card PlaceCardMove::getCard() const {
    return card;
}

int  CleanRowMove::getRowToClean() const {
    return rowToClean;
}
