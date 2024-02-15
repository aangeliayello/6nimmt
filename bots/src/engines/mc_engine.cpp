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
#include <memory>
#include <chrono>

using namespace std;
 
MCEngine::MCEngine(int n_simul) : n_simul(n_simul) {}

PlaceCardMove* MCEngine::makeDecisionPlaceCard(const Game& game, const vector<Card>& hand) {
    auto start = std::chrono::high_resolution_clock::now();
    Board board = game.getBoard();
    int n_players = game.getNumberOfPlayers();
    int cards_in_hand = hand.size();
    int n = ceil(float(n_simul) / hand.size());
    unordered_set<int> cardsToExclude = getSeenCards(game, hand);

    // Deck of cards without seen cards
    vector<Card> deck;
    vector<Card> shuffledDeck;
    for (int i = 1; i <= TOTAL_CARDS; ++i) {
        if (cardsToExclude.find(i) == cardsToExclude.end()) {
            deck.emplace_back(i, calculateBullHeads(i));
        }
    }

    Card bestCard = hand[0];
    float bestCardLoss = 999;
    // Simulate games for each card in hand to see which one is better
    vector<float> cardLosses(hand.size(), 0);
    int cardIdx;
    for (int simul = 0; simul < n; simul++){
        // Start simulation of a game
        shuffledDeck.clear();
        sample(deck.begin(), deck.end(), back_inserter(shuffledDeck), (n_players - 1) * cards_in_hand, rng);
        // Add players with RandomEngine to simulate game
        
        cardIdx = 0;
        for (auto c : hand){
            Game currGame;
            currGame.setBoard(board); // Copy board from current state
            // Add players with RandomEngine to simulate game
            
            RandomEngine* engine = new RandomEngine(true);
            currGame.addPlayer(make_unique<Player>("mc", engine));
            for (int i = 1; i < n_players; i++){
                RandomEngine* engine = new RandomEngine(true);
                currGame.addPlayer(make_unique<Player>("random" + to_string(i), engine));
            }
            vector<unique_ptr<Player>>& currPlayers = currGame.getPlayersV2();
            
            // Distribute random hands to the other players
            
            for (int i = 1; i < currPlayers.size(); i++){
                vector<Card> tempHand(shuffledDeck.begin() + (i-1)*cards_in_hand, shuffledDeck.begin() + (i)*cards_in_hand);
                currPlayers[i]->setHand(tempHand);
            }
            currGame.setGameStarted(true);

            // First round of car collection / processing

            // We choose the card that we want to simulate
            currGame.clearRoundMoves();
            PlaceCardMove* myMove = new PlaceCardMove(c);
            currGame.AddToRoundMoves(myMove);

            Player* me = currPlayers.front().get();
            vector<Card> myHand = hand;
            removeCardFromHand(c, myHand); // Remove that card from owr hand
            me->setHand(myHand);

            // Add moves to the game round
            for (int i = 1; i < currPlayers.size(); i++) {
                PlaceCardMove* move = currPlayers[i]->makeDecisionPlaceCard(currGame);
                currGame.AddToRoundMoves(move);
            }
            currGame.processMoves(); // Process the round

            // Run rest of the game randomly automatically
            currGame.runGame();

            vector<int> currScores = currGame.getScores();
            float currLoss = scoresToLoss(currScores);
            cardLosses[cardIdx] = cardLosses[cardIdx] + (currLoss - cardLosses[cardIdx])/(simul + 1);
            cardIdx++;
        }
    }

    for (int i = 0; i < cardLosses.size(); i++){
        float cardLoss = cardLosses[i];
        // Check if simulated card had a better loss
        if (cardLoss < bestCardLoss){
            bestCardLoss = cardLoss;
            bestCard = hand[i];
        }
    }

    float loss = scoresToLoss(game.getScores());
    cout << "Curr Loss: " << loss + bestCardLoss << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << "NS: " << n_simul << std::endl;

    // Create and return the move
    return new PlaceCardMove(bestCard);
}


// CleanRowMove* MCEngine::makeDecisionCleanRow(const Game& game, const vector<Card>& hand) {
//     vector<int> bullsPerRow = game.getBoard().getBullPerRow();
//     auto minElementIterator = min_element(bullsPerRow.begin(), bullsPerRow.end());
//     int chosenRow = distance(bullsPerRow.begin(), minElementIterator);
//     // TODO: For now just take the minimum, could also simulate from here

//     // Create and return the 
//     return new CleanRowMove(chosenRow);
// }

CleanRowMove* MCEngine::makeDecisionCleanRow(const Game& game, const vector<Card>& hand) {

    auto start = std::chrono::high_resolution_clock::now();
    Board board = game.getBoard();
    int n_players = game.getNumberOfPlayers();
    int n_rows = board.getNumberOfRows();
    int cards_in_hand = hand.size();
    int n = ceil(float(n_simul) / (hand.size()*n_rows));
    vector<PlaceCardMove*> roundMoves = game.getRoundMoves();
    unordered_set<int> cardsToExclude = getSeenCards(game, hand);
    for (auto m : roundMoves){
        cardsToExclude.insert(m->getCard().getNumber());
    }
    // Deck of cards without seen cards
    vector<Card> deck;
    vector<Card> shuffledDeck;
    for (int i = 1; i <= TOTAL_CARDS; ++i) {
        if (cardsToExclude.find(i) == cardsToExclude.end()) {
            deck.emplace_back(i, calculateBullHeads(i));
        }
    }

    CleanRowMove* cleanRowMove;
    // Simulate games for each card in hand to see which one is better
    vector<float> cardLosses(hand.size()*n_rows, 0);
    int cardIdx;
    for (int simul = 0; simul < n; simul++){
        // Start simulation of a game
        shuffledDeck.clear();
        sample(deck.begin(), deck.end(), back_inserter(shuffledDeck), (n_players - 1) * cards_in_hand, rng);
        // Add players with RandomEngine to simulate game
        
        cardIdx = 0;
        for (int r = 0; r < n_rows; r++){
            for (auto c : hand){
                Game currGame;
                currGame.setBoard(board); // Copy board from current state
                // Add players with RandomEngine to simulate game
                currGame.AddCleanRowMove(new CleanRowMove(r));
                for (auto rm : game.getRoundMoves()){
                    currGame.AddToRoundMoves(new PlaceCardMove(rm->getCard()));
                }
                currGame.processMovesWithCleaning();
                RandomEngine* engine = new RandomEngine(true);
                currGame.addPlayer(make_unique<Player>("mc", engine));
                for (int i = 1; i < n_players; i++){
                    RandomEngine* engine = new RandomEngine(true);
                    currGame.addPlayer(make_unique<Player>("random" + to_string(i), engine));
                }
                vector<unique_ptr<Player>>& currPlayers = currGame.getPlayersV2();
                
                // Distribute random hands to the other players
                
                for (int i = 1; i < currPlayers.size(); i++){
                    vector<Card> tempHand(shuffledDeck.begin() + (i-1)*cards_in_hand, shuffledDeck.begin() + (i)*cards_in_hand);
                    currPlayers[i]->setHand(tempHand);
                }
                currGame.setGameStarted(true);

                // First round of car collection / processing

                // We choose the card that we want to simulate
                currGame.clearRoundMoves();
                PlaceCardMove* myMove = new PlaceCardMove(c);
                currGame.AddToRoundMoves(myMove);

                Player* me = currPlayers.front().get();
                vector<Card> myHand = hand;
                removeCardFromHand(c, myHand); // Remove that card from owr hand
                me->setHand(myHand);

                // Add moves to the game round
                for (int i = 1; i < currPlayers.size(); i++) {
                    PlaceCardMove* move = currPlayers[i]->makeDecisionPlaceCard(currGame);
                    currGame.AddToRoundMoves(move);
                }
                currGame.processMoves(); // Process the round

                // Run rest of the game randomly automatically
                currGame.runGame();

                vector<int> currScores = currGame.getScores();
                float currLoss = scoresToLoss(currScores);
                cardLosses[cardIdx] = cardLosses[cardIdx] + (currLoss - cardLosses[cardIdx])/(simul + 1);
                cardIdx++;
            }
        }   
    }

    int bestRow = 0;
    float bestRowLoss = 999;
    for (int i = 0; i < cardLosses.size(); i++){
        float rowLoss = cardLosses[i];
        // Check if simulated card had a better loss
        if (rowLoss < bestRowLoss){
            bestRowLoss = rowLoss;
            bestRow = i/hand.size();
        }
    }

    float loss = scoresToLoss(game.getScores());
    cout << "Curr Loss: " << loss + bestRowLoss << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << "NS: " << n_simul << std::endl;

    // Create and return the move
    return new CleanRowMove(bestRow);
}