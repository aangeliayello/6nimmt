#ifndef CLICK_HANDLER_H
#define CLICK_HANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "graphics_handler.h"

class ClickHandler{
public:
    ClickHandler(sf::RenderWindow& window_ , std::vector<sf::Sprite>& handSprites, const std::vector<std::vector<sf::Sprite>>& boardSprites);
    
    void processClick(GraphicsHandler& graphicsHandler, bool waitingForInputToCleanRow); // TODO: Clean up use of the graphics handlers?

    void processEnter();

    void reset();
    void resetToCleanRowIndex();

    int getSelectedCardIndex() const;
    int getSelectedRowIndex() const;
    int getToPlayCardIndex() const;
    int& getToPlayCardIndexRef(); //TODO: :Only the clickhandler should be modifiying this value;
    int getToCleanRowIndex() const;

private:
    sf::RenderWindow& window;
    std::vector<sf::Sprite>& handSprites;
    const std::vector<std::vector<sf::Sprite>>& boardSprites;
    int selectedCardIndex;
    int selectedRowIndex;
    int cardIndexToPlay;
    int rowIndexToClean;
};


#endif