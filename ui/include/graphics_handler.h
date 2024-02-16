#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "../include/card_texture.h"
#include "../../bots/include/core/game.h"

class GraphicsHandler {
public:
    GraphicsHandler(sf::RenderWindow& window_, const Game& game);
    void updateScaleFactorAndCardSize();
    void updateBackgroundSprite();
    void setCardsToSprites(vector<sf::Sprite>& sprites, const vector<int>& values);
    void setHand(const std::vector<int>& handValues);
    void setBoard(const std::vector<std::vector<int>>& boardValues);
    void drawOutline(const sf::Sprite& card, sf::Color color);
    void updateHandSprites(const Game& game);
    void updateBoardSprites(const Game& game);
    void updateShownCards(const Game& game);
    void updateSelectionIndicator(const int selectedRowIndex, const Board& board);
    void updateScoreBoard(const std::vector<int>& scores, const std::vector<std::unique_ptr<Player>>& players);
    void updateExpectedMoveMessage(bool waitingForInputToPlayCard, bool waitingForInputToCleanRow);
    void updateGameOverMsg(const std::string& winner);

    const std::vector<sf::Sprite>& getHandSprites() const;
    std::vector<sf::Sprite>& getHandSpritesRef();
    const std::vector<vector<sf::Sprite>>& getBoardSprites() const;
    const sf::Sprite& getBackgroundSprite() const;

    bool handleCardSelection(int& selectedCardIndex);

private:
    sf::RenderWindow& window;

    sf::Texture backgroundTexture;
    
    std::vector<sf::Sprite> handSprites;
    std::vector<std::vector<sf::Sprite>> boardSprites;
    std::vector<sf::Sprite> shownCardsSprites;
    sf::Sprite selectionIndicatorSprite;
    sf::Sprite backgroundSprite;

    std::vector<std::unique_ptr<CardTexture>> cardTextures;
    sf::Font titleFont;
    sf::Font textFont;

    float originalCardWidth;
    float originalCardHeight;

    float scaleFactor;
    float scaleFactorX;
    float scaleFactorY;
    float cardWidth;
    float cardHeight;

    
    sf::Vector2f originalPosition; //TODO: This is a hack to handle card selection displacement
};

#endif