#ifndef GAME_FACTORY_H
#define GAME_FACTORY_H

#include "../include/core/game.h"
#include "../include/core/player.h"
#include "../include/core/card.h"
#include "../include/globals.h"

void startGame(Game& game);

vector<vector<int>> getBoard(const Game& game);

vector<int> getHand(const Game& game);

#endif