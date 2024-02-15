#include <SFML/Graphics.hpp>
#include <memory>

#include "../include/globals.h"
#include "../include/assets.h"
#include "../include/card_texture.h"
#include "../include/graphics_handler.h"
#include "../../bots/include/core/game.h"
#include "../../bots/include/game_factory.h"

// Global variables
bool handleRowSelection(const std::vector<std::vector<sf::Sprite>>& board, const sf::RenderWindow& window, int& selectedRowIndex) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);

    for (int row = 0; row < board.size(); ++row) {
        for (const auto& card : board[row]) {
            if (card.getGlobalBounds().contains(worldPos)) {
                selectedRowIndex = row;
                return true; // Stop checking once a row has been selected
            }
        }
    }

    // If the click was outside any row, unselect row
    selectedRowIndex = -1;

    return false;
}

void deleteCardFromHand(std::vector<sf::Sprite>& hand, std::vector<std::vector<sf::Sprite>>& board, int& selectedCardIndex){
    if (selectedCardIndex != -1) {
        // Remove the selected card from the hand
        board[0].push_back(hand[selectedCardIndex]);
        hand.erase(hand.begin() + selectedCardIndex);

        selectedCardIndex = -1; // Reset selected card index
    }
}

void deleteAllButLastInRow(std::vector<std::vector<sf::Sprite>>& board, int& selectedRowIndex) {
    if (selectedRowIndex >= 0 && selectedRowIndex < board.size()) {
        if (board[selectedRowIndex].size() > 1) { // Ensure there's more than one card to delete
            // Keep the last card, remove the rest
            sf::Sprite lastCard = board[selectedRowIndex].back();
            board[selectedRowIndex].clear();
            board[selectedRowIndex].push_back(lastCard);
        }
        selectedRowIndex = -1; // Reset row selection
    }
}

void chooseCardMove(int& selectedCardIndex, int& cardIndexToPlay){
    if (selectedCardIndex != -1) {
        // Remove the selected card from the hand
        cardIndexToPlay = selectedCardIndex;
        selectedCardIndex = -1; // Reset selected card index
    }
}

void chooseCleanRowMove(int& selectedRowIndex, int& rowIndexToClean){
    if (selectedRowIndex != -1) {
        // Remove the selected card from the hand
        rowIndexToClean = selectedRowIndex;
        selectedRowIndex = -1; // Reset selected card index
    }
}


int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width*0.7, desktop.height*0.7), "Zugma");

    // Backend
    Game game = Game();
    startGame(game);

    GraphicsHandler graphicsHandler(window, game);


    // Set positions for each card in the grid
    int selectedCardIndex = -1;
    int cardIndexToPlay = -1;
    int selectedRowIndex = -1;
    int rowIndexToClean = -1;
    bool handSelectedInClick = false;
    bool rowSelectedInClick = false;
    bool needCleaning = false;
    bool waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
    bool waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
    bool waitingForBotMoves = (!waitingForInputToCleanRow) && (!waitingForInputToPlayCard);
    bool gameOver = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                graphicsHandler.updateScaleFactorAndCardSize();
                graphicsHandler.updateBackgroundSprite();
                graphicsHandler.updateHandSprites(game);
                graphicsHandler.updateBoardSprites(game);
            }

            waitingForBotMoves = !waitingForInputToCleanRow && !waitingForInputToPlayCard;
        
            if (waitingForBotMoves){
                // Bots can play
                if (game.getPlayers().front()->getHand().size()){
                    game.collectMovesBots();
                    waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
                    waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
                } else{
                    waitingForInputToCleanRow = false;
                    waitingForInputToPlayCard = false;
                    gameOver = true;
                }
            } 

            // Place Card
            if (waitingForInputToPlayCard && (cardIndexToPlay != -1)){
                game.inputCardToPlay(cardIndexToPlay);
                graphicsHandler.updateHandSprites(game);

                bool cleanRowInputNeed = game.checkIfInputToCleanRowNeeded();
                if (!cleanRowInputNeed){
                    game.processMoves();
                    graphicsHandler.updateBoardSprites(game);
                } 
                cout << "Scores: " << endl;
                game.printScores();
                waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
                waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
            }

            // Clean row
            if (waitingForInputToCleanRow){
                if (rowIndexToClean != -1){
                    game.AddCleanRowMove(new CleanRowMove(rowIndexToClean));
                    rowIndexToClean = -1;
                    game.processMovesWithCleaning();
                    graphicsHandler.updateBoardSprites(game);
                    cout << "Scores: " << endl;
                    game.printScores();
                    waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
                    waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    // Check if a card is selected

                    chooseCardMove(selectedCardIndex, cardIndexToPlay);
                    chooseCleanRowMove(selectedRowIndex, rowIndexToClean);
                }

                if ((event.key.code == sf::Keyboard::R) && 
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))){
                    cout << "RESET" << endl;
                    game.resetGame();
                    selectedCardIndex = -1;
                    cardIndexToPlay = -1;
                    selectedRowIndex = -1;
                    rowIndexToClean = -1;
                    gameOver = false;
                    waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
                    waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
                    graphicsHandler.updateHandSprites(game);
                    graphicsHandler.updateBoardSprites(game);
                    break;
                }  
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!waitingForInputToCleanRow){
                        handSelectedInClick = graphicsHandler.handleCardSelection(selectedCardIndex);
                    } else{
                        rowSelectedInClick = handleRowSelection(graphicsHandler.getBoardSprites(), window, selectedRowIndex);
                    }
                }
            }
        }

        window.clear();
        window.draw(graphicsHandler.getBackgroundSprite());
        int cardIdx = 0;
        sf::Color color;
        for (const auto& card : graphicsHandler.getHandSprites()) {
            if (cardIdx == selectedCardIndex){
                color = sf::Color::Red;
            } else{
                color = sf::Color::Black;
            }
            graphicsHandler.drawOutline(card, color);
            window.draw(card);
            cardIdx++;
        }

        for (const auto& row : graphicsHandler.getBoardSprites()) {
            for (const auto& card : row) {
                graphicsHandler.drawOutline(card, sf::Color::Black);
                window.draw(card);
            }
        }
        if (waitingForInputToCleanRow){
            graphicsHandler.updateShownCards(game);
        }
        graphicsHandler.updateExpectedMoveMessage(waitingForInputToPlayCard, waitingForInputToCleanRow);
        graphicsHandler.updateScoreBoard(game.getScores(), game.getPlayers());
        graphicsHandler.updateSelectionIndicator(selectedRowIndex, game.getBoard());
        if (gameOver){
            graphicsHandler.updateGameOverMsg(game.getWinner());
        }
        window.display();
    }

    return 0;
}
