#include "../../include/engines/mcts_engine.h"
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
#include <tuple>

using namespace std;
 
MCTSEngine::MCTSEngine(int n_simul, int n_init_simul, float exploration_fraction) : n_simul(n_simul), n_init_simul(n_init_simul), exploration_fraction(exploration_fraction) {}

vector<float> simulationBody(int n_simul, vector<Card> cards, Board board, vector<Card> deck, Engine* engine, int n_players, vector<Card> hand){
    vector<Card> shuffledDeck;
    vector<float> cardLosses(cards.size(), 0);
    int n = (n_simul + cards.size() -1)/cards.size();
    int cardIdx;
    for (int simul = 0; simul < n; simul++){
        int cards_in_hand = hand.size();
        // Start simulation of a game
        shuffledDeck.clear();
        sample(deck.begin(), deck.end(), back_inserter(shuffledDeck), (n_players - 1) * cards_in_hand, rng);
        // Add players with RandomEngine to simulate game
        
        cardIdx = 0;
        for (auto c : cards){
            Game currGame;
            currGame.setBoard(board); // Copy board from current state
            RandomEngine* engine = new RandomEngine(true);
            currGame.addPlayer(make_unique<Player>("mc", engine));
            for (int i = 1; i < n_players; i++){
                RandomEngine* engine = new RandomEngine(true);
                currGame.addPlayer(make_unique<Player>("random" + to_string(i), engine));
            }
            const vector<unique_ptr<Player>>& currPlayers = currGame.getPlayers(); 

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

            vector<Card> myHand = hand;
            removeCardFromHand(c, myHand); // Remove that card from owr hand
            currPlayers[0]->setHand(myHand);

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

    return cardLosses;
}

PlaceCardMove* MCTSEngine::makeDecisionPlaceCard(const Game& game, const vector<Card>& hand) {

    // Board board = game.getBoard();
    // const vector<unique_ptr<Player>>& players = game.getPlayers();//
    // int n_players = players.size();
    // int cards_in_hand = hand.size();
    // int n_exploration = ceil(exploration_fraction * cards_in_hand);
    // unordered_set<int> cardsToExclude = getSeenCards(game, hand);

    // // Deck of cards without seen cards
    // vector<Card> deck;
    // for (int i = 1; i <= TOTAL_CARDS; ++i) {
    //     if (cardsToExclude.find(i) == cardsToExclude.end()) {
    //         deck.emplace_back(i, calculateBullHeads(i));
    //     }
    // }

    Card bestCard = hand[0];
    // float bestCardLoss = 999;
    // vector<tuple<int, float>> cardAvgLosses;

    // RandomEngine* engine = new RandomEngine(true);
    // int cardIdx = 0;
    // // Simulate games for each card in hand to see which one is better
    // vector<float> initCardLosses = simulationBody(n_init_simul, hand, board, deck, engine, n_players, hand);
    // for (int i = 0; i < cards_in_hand; i++){
    //     cardAvgLosses.push_back(make_tuple(i, initCardLosses[i]));
    // }

    // std::sort(cardAvgLosses.begin(), cardAvgLosses.end(), [](const tuple<int, float>& a, const tuple<int, float>& b) {
    //     return get<1>(a) < get<1>(b);
    // });

    // // cout << "Init Phase: "<< endl;
    // // for (auto il : cardAvgLosses){
    // //     cout << hand[get<0>(il)].toString() << " " << get<1>(il) << endl;
    // // }
    // // cout << endl;

    // vector<tuple<int, float>> bestCardsIdxLoss(cardAvgLosses.begin(), cardAvgLosses.begin() + n_exploration);
    // vector<Card> cards;
    // for (auto t : cardAvgLosses){
    //     cards.push_back(hand[get<0>(t)]);
    // }

    // vector<float> cardLosses = simulationBody(n_simul - n_init_simul, cards, board, deck, engine, n_players, hand);

    // float exp_frac = float(n_init_simul)/n_simul;

    // // cout << "Second Phase" << endl;
    // for (int i = 0; i < bestCardsIdxLoss.size(); i++){
    //     int idx = get<0>(bestCardsIdxLoss[i]);
    //     float initLoss = get<1>(bestCardsIdxLoss[i]);
    //     float currLoss = cardLosses[i];
    //     float cardLoss = initLoss * exp_frac + currLoss*(1 - exp_frac);
    //     // cout << hand[get<0>(bestCardsIdxLoss[i])].toString() << " " << cardLoss << endl;

    //     // Check if simulated card had a better loss
    //     if (cardLoss < bestCardLoss){
    //         bestCardLoss = cardLoss;
    //         bestCard = cards[i];
    //     }
    // }
    // cout << endl;

    // Create and return the move
    return new PlaceCardMove(bestCard);
}

CleanRowMove* MCTSEngine::makeDecisionCleanRow(const Game& game, const vector<Card>& hand) {
    vector<int> bullsPerRow = game.getBoard().getBullPerRow();
    auto minElementIterator = min_element(bullsPerRow.begin(), bullsPerRow.end());
    int chosenRow = distance(bullsPerRow.begin(), minElementIterator);
    // TODO: For now just take the minimum, could also simulate from here

    // Create and return the move
    return new CleanRowMove(chosenRow);
}
