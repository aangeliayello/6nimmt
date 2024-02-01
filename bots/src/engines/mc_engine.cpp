#include "../../include/engines/mc_engine.h"
#include "../../include/engines/random_engine.h"
#include "../../include/core/move.h"
#include "../../include/core/game.h"
#include "../../include/core/player.h"
#include "../../include/globals.h"
#include "../../include/utils.h"
#include <algorithm>
#include <cmath> 
#include <unordered_set>
#include <string>

using namespace std;
 
MCEngine::MCEngine(int n_simul) : n_simul(n_simul) {}

unordered_set<int> getSeenCards(const Game& game, const vector<Card>& hand){
    unordered_set<int> seenCards = {};
    // TODO: also add cards in the history

    // Add cards in the board
    for (auto& row : game.getBoard().getRows()){
        for (auto& c : row){
            seenCards.insert(c.getNumber());
        }
    }

    // Add cards in the player's hand
    for (auto& c : hand){
        seenCards.insert(c.getNumber());
    }

    return seenCards;
};

float scoresToLoss(vector<int> scores){
    // Assuming 0th position is our score

    float loss = scores[0];
    for (int i = 1; i < scores.size(); i++){
        loss += -scores[i];
    }

    return loss/scores.size();
}

Move* MCEngine::makeDecision(const Game& game, const vector<Card>& hand) {
    if (game.getExpectingCleanRowMove()){
        vector<int> bullsPerRow = game.getBoard().getBullPerRow();
        auto minElementIterator = min_element(bullsPerRow.begin(), bullsPerRow.end());
        int chosenRow = distance(bullsPerRow.begin(), minElementIterator);
        // TODO: For now just take the minimum, could also simulate from here

        // Create and return the move
        return new CleanRowMove(chosenRow);
    } else {
        Board board = game.getBoard();
        vector<Player*> players = game.getPlayers();

        int n = ceil(float(n_simul) / hand.size());
        unordered_set<int> cardsToExclude = getSeenCards(game, hand);

        // Deck of cards without seen cards
        vector<Card> deck;
        for (int i = 1; i <= TOTAL_CARDS; ++i) {
            if (cardsToExclude.find(i) == cardsToExclude.end()) {
                deck.emplace_back(i, calculateBullHeads(i));
            }
        }

        Card bestCard = hand[0];
        float bestCardLoss = 999;

        // Simulate games for each card in hand to see which one is better
        for (auto& c : hand){
            // Vector to keep track of the results from the simulation
            vector<float> cardLosses;

            for (int simul = 0; simul < n; simul++){
                // Start simulation of a game
                Game currGame;
                currGame.setBoard(board); // Copy board from current state
            
                shuffle(begin(deck), end(deck), rng); // shuffle the deck

                // Add players with RandomEngine to simulate game
                currGame.addPlayer(new Player("mc", new RandomEngine(true)));
                for (int i = 1; i < players.size(); i++){
                    currGame.addPlayer(new Player("random" + to_string(i), new RandomEngine(true)));
                }
                vector<Player*> currPlayers = currGame.getPlayers();

                // Distribute random hands to the other players
                vector<Card> tempHand;
                for (int i = 1; i < currPlayers.size(); i++){
                    for (int j = 0; j < hand.size(); j++){
                        tempHand.push_back(deck[(i-1)*hand.size() + j]);
                    }
                    currPlayers[i]->setHand(tempHand);
                    tempHand.clear();
                }
                currGame.setGameStarted(true);

                // First round of car collection / processing

                // We choose the card that we want to simulate
                currGame.clearRoundMoves();
                Move* myMove = new PlaceCardMove(c);
                currGame.AddToRoundMoves(myMove);

                Player* me = currPlayers[0];
                vector<Card> myHand = hand;
                removeCardFromHand(c, myHand); // Remove that card from owr hand
                me->setHand(myHand);

                // Add moves to the game round
                for (int i = 1; i < currPlayers.size(); i++) {
                    Move* move = currPlayers[i]->makeDecision(currGame);
                    currGame.AddToRoundMoves(move);
                }
                currGame.processMoves(); // Process the round

                // Run rest of the game randomly automatically
                currGame.runGame();

                vector<int> currScores = currGame.getScores();
                float currLoss = scoresToLoss(currScores);
                cardLosses.push_back(currLoss);
            }
            float cardLoss = accumulate(cardLosses.begin(), cardLosses.end(), 0.0) / cardLosses.size();

            // Check if simulated card had a better loss
            if (cardLoss < bestCardLoss){
                bestCardLoss = cardLoss;
                bestCard = c;
            }
        }
        // Create and return the move
        return new PlaceCardMove(bestCard);
    }
}