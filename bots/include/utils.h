#ifndef UTILS_H
#define UTILS_J

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

#endif