#include <SFML/Graphics.hpp>
#include <memory>

#include "../include/globals.h"
#include "../include/assets.h"
#include "../include/card_texture.h"
#include "../include/graphics_handler.h"
#include "../include/click_handler.h"
#include "../../bots/include/core/game.h"
#include "../../bots/include/game_factory.h"


int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width*0.7, desktop.height*0.7), "Zugma");

    // Backend
    Game game = Game();
    startGame(game);

    GraphicsHandler graphicsHandler(window, game);
    ClickHandler clickHandler(window, graphicsHandler.getHandSpritesRef(), graphicsHandler.getBoardSprites());

    // Set positions for each card in the grid
    bool waitingForInputToCleanRow = game.getWaitingForInputToCleanRow(); //TODO: This lines should be handle by a stateHandler class
    bool waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
    bool waitingForBotMoves = (!waitingForInputToCleanRow) && (!waitingForInputToPlayCard);
    bool gameOver = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea)); // TODO: this should be done by the graphics handler
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
            if (waitingForInputToPlayCard && (clickHandler.getToPlayCardIndex() != -1)){
                game.inputCardToPlay(clickHandler.getToPlayCardIndexRef() ); //TODO: Need to split this function, too much stuff is happing inside inputCardToPlay
                graphicsHandler.updateHandSprites(game);

                if (!game.checkIfInputToCleanRowNeeded()){
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
                if (clickHandler.getToCleanRowIndex() != -1){
                    game.AddCleanRowMove(new CleanRowMove(clickHandler.getToCleanRowIndex()));
                    clickHandler.resetToCleanRowIndex();
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
                    clickHandler.processEnter();
                }

                if ((event.key.code == sf::Keyboard::R) && 
                    (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))){
                    cout << "RESET" << endl;
                    game.resetGame();
                    clickHandler.reset();
                    gameOver = false;
                    waitingForInputToCleanRow = game.getWaitingForInputToCleanRow();
                    waitingForInputToPlayCard = game.getWaitingForInputToPlayCard();
                    graphicsHandler.updateHandSprites(game);
                    graphicsHandler.updateBoardSprites(game);
                    break;
                }  
            }
            if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                clickHandler.processClick(graphicsHandler, waitingForInputToCleanRow);
            }
        }

        window.clear();
        window.draw(graphicsHandler.getBackgroundSprite());
        int cardIdx = 0;
        sf::Color color;
        for (const auto& card : graphicsHandler.getHandSprites()) {
            if (cardIdx == clickHandler.getSelectedCardIndex()){
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
        graphicsHandler.updateSelectionIndicator(clickHandler.getSelectedRowIndex(), game.getBoard());
        if (gameOver){
            graphicsHandler.updateGameOverMsg(game.getWinner());
        }
        window.display();
    }

    return 0;
}
