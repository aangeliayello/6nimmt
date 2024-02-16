#include "../include/graphics_handler.h"

#include "../include/globals.h"
#include "../include/assets.h"
#include "../../bots/include/game_factory.h"
#include "../../bots/include/globals.h"

GraphicsHandler::GraphicsHandler(sf::RenderWindow& window_, const Game& game) : window(window_) {
    sf::Image icon = loadIcon();
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Card Textures
    if (TOTAL_CARDS > 104) throw std::runtime_error("TOTAL_CARDS exceeded the maximum of 104 cards.");
    
    for (int i = 0; i < TOTAL_CARDS; ++i) {
        this->cardTextures.push_back(std::make_unique<CardTexture>(i+1));
    }

    // Fonts
    this->titleFont = loadTitleFont();
    this->textFont = loadTextFont();

    this->originalCardWidth = cardTextures[0]->getTexture().getSize().x;
    this->originalCardHeight = cardTextures[0]->getTexture().getSize().y;
    
    updateScaleFactorAndCardSize();
    
    this->backgroundTexture = loadBackgroundTexture();
    this->backgroundSprite.setTexture(backgroundTexture);
    this->backgroundSprite.setScale(scaleFactorX, scaleFactorY);


    // Hand
    updateHandSprites(game);

    // Board
    updateBoardSprites(game);
}

void GraphicsHandler::updateScaleFactorAndCardSize(){
    this->scaleFactorX = window.getSize().x / BASE_WIDTH;
    this->scaleFactorY = window.getSize().y / BASE_HEIGHT;
    this->scaleFactor = min(scaleFactorX, scaleFactorY);
    this->cardWidth = originalCardWidth * scaleFactor;
    this->cardHeight = originalCardHeight * scaleFactor;
    // TODO: also update Origin
}

void GraphicsHandler::updateBackgroundSprite(){
    backgroundSprite.setScale(scaleFactorX, scaleFactorY);
}

void GraphicsHandler::setCardsToSprites(vector<sf::Sprite>& sprites, const vector<int>& values){
    sprites.clear();
    sprites.resize(values.size());
    for (int i = 0; i < values.size(); ++i) {
        int cardIdx = values[i]-1;
        sprites[i].setTexture(cardTextures[cardIdx]->getTexture());
        sprites[i].setScale(scaleFactor, scaleFactor);
    }
}

void GraphicsHandler::setHand(const vector<int>& handValues) {
    setCardsToSprites(handSprites, handValues);
}

void GraphicsHandler::setBoard(const std::vector<std::vector<int>>& boardValues){
    boardSprites.clear();
    boardSprites.resize(boardValues.size());
    for (int row = 0; row < boardValues.size(); row++) {
        boardSprites[row].reserve(boardValues[row].size());
        for (int col = 0; col < boardValues[row].size(); col++) {
            int cardIdx = boardValues[row][col]-1;
            sf::Sprite sprite;
            sprite.setTexture(cardTextures[cardIdx]->getTexture());
            sprite.setScale(scaleFactor, scaleFactor);

            boardSprites[row].push_back(std::move(sprite));
        }
    }
}

void GraphicsHandler::drawOutline(const sf::Sprite& card, sf::Color color = sf::Color::Black){
    float thickness = 2;
    sf::Vector2f cardSize(card.getGlobalBounds().width, card.getGlobalBounds().height);
    sf::Vector2f outlineSize(cardSize.x + 2* thickness, cardSize.y + 2*thickness);
    sf::RectangleShape cardOutline(outlineSize);

    cardOutline.setPosition(card.getPosition().x - thickness, card.getPosition().y - thickness);
    cardOutline.setFillColor(color); // Or any color for the outline
    cardOutline.setOutlineThickness(0);

    window.draw(cardOutline);
}

void GraphicsHandler::updateHandSprites(const Game& game) {
    vector<int> handValues = getHand(game);
    setHand(handValues);
    
    // Calculate the total width of all cards and spacing
    float horizontalSpacing = HORIZONTAL_SPACING*scaleFactor;
    float verticalSpacing = VERTICAL_SPACING*scaleFactor;
    float totalWidth = (cardWidth + horizontalSpacing) * handValues.size() - horizontalSpacing;
    float startXHand = (window.getSize().x - totalWidth) / 2; // Center horizontally
    float startYHand = window.getSize().y - cardHeight - verticalSpacing; // Assuming cardHeight is known and 20 pixels padding from bottom

    for (int i = 0; i < handValues.size(); ++i) {
        handSprites[i].setPosition(startXHand + i * (cardWidth + horizontalSpacing), startYHand);
    }
}

void GraphicsHandler::updateBoardSprites(const Game& game) {
    
    vector<vector<int>> boardValues = getBoard(game);
    setBoard(boardValues);

    // Calculate the total width of all cards and spacing
    float horizontalSpacing  = HORIZONTAL_SPACING*scaleFactor;
    float verticalSpacing = VERTICAL_SPACING*scaleFactor;
    float startXBoard= horizontalSpacing*2; //(window.getSize().x - totalGridWidth) / 2;
    float startYBoard= verticalSpacing*2; //(window.getSize().y - totalGridHeight) / 2;

    for (int row = 0; row < boardValues.size(); ++row) {
        for (int col = 0; col < boardValues[row].size(); ++col) {
            int index = row * boardValues[row].size() + col;
            float x = startXBoard+ col * (cardWidth + horizontalSpacing);
            float y = startYBoard+ row * (cardHeight + verticalSpacing);
            boardSprites[row][col].setPosition(x, y);
        }
    }
}

void GraphicsHandler::updateShownCards(const Game& game) {
    
    vector<int> cardsToShow = getShownCards(game);
    setCardsToSprites(shownCardsSprites, cardsToShow);

    // Calculate the total width of all cards and spacing
    float horizontalSpacing  = HORIZONTAL_SPACING*scaleFactor;
    float verticalSpacing = VERTICAL_SPACING*scaleFactor;
    float startYBoard= verticalSpacing*5; //(window.getSize().y - totalGridHeight) / 2;

    sf::Text text;
    text.setFont(titleFont);
    text.setCharacterSize(14); // Adjust size as needed
    text.setFillColor(sf::Color::White); // Inner color
    text.setOutlineColor(sf::Color::Black); // Border color
    text.setOutlineThickness(2); // Adjust thickness as needed
    text.setStyle(sf::Text::Bold); // Set text style
    text.setString("Players Cards:");
    
    sf::FloatRect textRect = text.getLocalBounds();
    float startXBoard= window.getSize().x - 5*horizontalSpacing - std::max(cardsToShow.size()*(cardWidth + horizontalSpacing), textRect.getSize().x); //(window.getSize().x - totalGridWidth) / 2;
    float startYCards = startYBoard + verticalSpacing + textRect.getSize().y; 

    text.setPosition(sf::Vector2f(startXBoard, startYBoard));
    float x;
    for (int col = 0; col < cardsToShow.size(); ++col) {
        x = startXBoard+ col * (cardWidth + horizontalSpacing);
        shownCardsSprites[col].setPosition(x, startYCards);
    }
    
    const auto& players = game.getPlayers();

    float labelYPosition = startYCards + (cardHeight + verticalSpacing);
    vector<sf::Text> playerNamesLabels;
    int col = 0;
    for (const auto& player : players){
        sf::Text label;
        label.setFont(titleFont);
        label.setCharacterSize(8); // Adjust size as needed
        label.setFillColor(sf::Color::White); // Inner color
        label.setOutlineColor(sf::Color::Black); // Border color
        label.setOutlineThickness(2); // Adjust thickness as needed
        label.setString(player->getName());
        x = startXBoard+ col * (cardWidth + horizontalSpacing);
        label.setPosition(x, labelYPosition);
        playerNamesLabels.push_back(label);
        col++;
    }

    // Cards
    for (const auto& card : shownCardsSprites){
        drawOutline(card, sf::Color::Black);
        window.draw(card);
    }

    // Player labels
    for (auto& label : playerNamesLabels){
        window.draw(label);
    }

    // Title text
    window.draw(text);
}

void GraphicsHandler::updateSelectionIndicator(int& selectedRowIndex, const Board& board) {

    if (selectedRowIndex != -1) {
        float horizontalSpacing  = HORIZONTAL_SPACING*scaleFactor;
        float verticalSpacing = VERTICAL_SPACING*scaleFactor;

        // Calculate the total width of all cards and spacing
        float startXBoard= horizontalSpacing*2; 
        float startYBoard= verticalSpacing*2; 

        sf::Vector2f rectSize( int(horizontalSpacing/2),  int(cardHeight*0.8));
        sf::RectangleShape selectionIndicatorFront(rectSize);
        selectionIndicatorFront.setFillColor(sf::Color::Red); 

        // Position the circle in front of the row. Adjust x and y as needed.
        float xPosition = startXBoard - horizontalSpacing*1.5;
        float yPosition = startYBoard+ selectedRowIndex * (cardHeight + verticalSpacing) + 0.1*cardHeight;
        selectionIndicatorFront.setPosition(xPosition, yPosition);

        sf::RectangleShape selectionIndicatorEnd(rectSize);
        selectionIndicatorEnd.setFillColor(sf::Color::Red); 

        // Position the circle in front of the row. Adjust x and y as needed.
        int cardsInRow = board.getRows()[selectedRowIndex].size();
        xPosition = startXBoard + cardsInRow*(cardWidth + horizontalSpacing);
        yPosition = startYBoard+ selectedRowIndex * (cardHeight + verticalSpacing) + 0.1*cardHeight;
        selectionIndicatorEnd.setPosition(xPosition, yPosition);

        window.draw(selectionIndicatorFront);
        window.draw(selectionIndicatorEnd);
    }
}

void GraphicsHandler::updateScoreBoard(const vector<int>& scores, const vector<unique_ptr<Player>>& players) {
    string scoreText = "Scores:\n";
    for (int i  = 0; i < scores.size(); i++){
        scoreText += players[i]->getName() + ": " + to_string(scores[i]) + "\n";
    }

    // Calculate the total width of all cards and spacing
    float horizontalSpacing = HORIZONTAL_SPACING * scaleFactor;
    float verticalSpacing = VERTICAL_SPACING * scaleFactor;

    sf::Text text;
    text.setFont(textFont);
    text.setCharacterSize(14); // Adjust size as needed
    text.setFillColor(sf::Color::Black); // Inner color
    text.setStyle(sf::Text::Bold); // Set text style
    text.setString(scoreText);
    // Calculate the text's center position
    sf::FloatRect textRect = text.getLocalBounds();
    float yTopLeft = window.getSize().y - textRect.height - 3*verticalSpacing;
    float xTopLeft = 2*horizontalSpacing;
    text.setPosition(sf::Vector2f(xTopLeft, yTopLeft));

    window.draw(text);
}

void GraphicsHandler::updateExpectedMoveMessage(bool waitingForInputToPlayCard, bool waitingForInputToCleanRow) {
    if (waitingForInputToPlayCard || waitingForInputToCleanRow){

        // Common settings
        float verticalSpacing = VERTICAL_SPACING * scaleFactor;
        unsigned int characterSize = 14; // Adjust size as needed
        float outlineThickness = 2; // Adjust thickness as needed

        // Prepare texts
        sf::Text textPrefix;
        textPrefix.setFont(titleFont);
        textPrefix.setString("Select a ");
        textPrefix.setCharacterSize(characterSize);
        textPrefix.setFillColor(sf::Color::White);
        textPrefix.setOutlineColor(sf::Color::Black);
        textPrefix.setOutlineThickness(outlineThickness);
        textPrefix.setStyle(sf::Text::Bold);

        sf::Text textHighlight;
        textHighlight.setFont(titleFont);
        textHighlight.setString(waitingForInputToPlayCard ? "CARD" : "ROW");
        textHighlight.setCharacterSize(characterSize);
        textHighlight.setFillColor(waitingForInputToPlayCard ? sf::Color::Red : sf::Color::Blue); // Red for highlight
        textHighlight.setOutlineColor(sf::Color::Black);
        textHighlight.setOutlineThickness(outlineThickness);
        textHighlight.setStyle(sf::Text::Bold);

        sf::Text textSuffix;
        textSuffix.setFont(titleFont);
        textSuffix.setString(" and press ENTER.");
        textSuffix.setCharacterSize(characterSize);
        textSuffix.setFillColor(sf::Color::White);
        textSuffix.setOutlineColor(sf::Color::Black);
        textSuffix.setOutlineThickness(outlineThickness);
        textSuffix.setStyle(sf::Text::Bold);

        // Calculate positions
        sf::FloatRect prefixBounds = textPrefix.getLocalBounds();
        sf::FloatRect highlightBounds = textHighlight.getLocalBounds();
        sf::FloatRect suffixBounds = textSuffix.getLocalBounds();

        // Position calculation
        float startX = (window.getSize().x - (textPrefix.getLocalBounds().width + textHighlight.getLocalBounds().width + textSuffix.getLocalBounds().width)) / 2.0f;
        float startY = window.getSize().y - cardHeight - verticalSpacing * 2 - textPrefix.getLocalBounds().height;

        textPrefix.setPosition(startX, startY - textPrefix.getLocalBounds().height / 2 - textPrefix.getLocalBounds().top);
        startX += textPrefix.getLocalBounds().width;

        textHighlight.setPosition(startX, startY - textHighlight.getLocalBounds().height / 2 - textHighlight.getLocalBounds().top);
        startX += textHighlight.getLocalBounds().width;

        textSuffix.setPosition(startX, startY - textSuffix.getLocalBounds().height / 2 - textSuffix.getLocalBounds().top);

        // Draw the texts
        window.draw(textPrefix);
        window.draw(textHighlight);
        window.draw(textSuffix);
    }
}

void GraphicsHandler::updateGameOverMsg(const string& winner) {

    // Common settings
    float verticalSpacing = VERTICAL_SPACING * scaleFactor;
    unsigned int characterSize = 24; // Adjust size as needed
    float outlineThickness = 3; // Adjust thickness as needed

    // Prepare texts
    sf::Text textPrefix;
    textPrefix.setFont(titleFont);
    textPrefix.setString("GAME OVER!\n");

    textPrefix.setCharacterSize(characterSize);
    textPrefix.setFillColor(sf::Color::Cyan);
    textPrefix.setOutlineColor(sf::Color::Black);
    textPrefix.setOutlineThickness(outlineThickness);
    textPrefix.setStyle(sf::Text::Bold);

    sf::Text textHighlight;
    textHighlight.setFont(titleFont);
    textHighlight.setString(winner);
    textHighlight.setCharacterSize(characterSize);
    textHighlight.setFillColor(sf::Color::Magenta); // Red for highlight
    textHighlight.setOutlineColor(sf::Color::Black);
    textHighlight.setOutlineThickness(outlineThickness);
    textHighlight.setStyle(sf::Text::Bold);

    sf::Text textSuffix;
    textSuffix.setFont(titleFont);
    textSuffix.setString(" is the WINNER!");
    textSuffix.setCharacterSize(characterSize);
    textSuffix.setFillColor(sf::Color::White);
    textSuffix.setOutlineColor(sf::Color::Black);
    textSuffix.setOutlineThickness(outlineThickness);
    textSuffix.setStyle(sf::Text::Bold);

    // Calculate positions
    sf::FloatRect prefixBounds = textPrefix.getLocalBounds();
    sf::FloatRect highlightBounds = textHighlight.getLocalBounds();
    sf::FloatRect suffixBounds = textSuffix.getLocalBounds();

    // Position calculation
    float startXFirstLine = (window.getSize().x - (textPrefix.getLocalBounds().width)) / 2.0f;
    float startXSecondLine = (window.getSize().x - (textSuffix.getLocalBounds().width + textHighlight.getLocalBounds().width)) / 2.0f;
    
    float startYFirstLine = (window.getSize().y ) / 2.0f - textPrefix.getLocalBounds().height ;
    float startYSecondLine = (window.getSize().y ) / 2.0f + verticalSpacing;

    textPrefix.setPosition(startXFirstLine, startYFirstLine);

    textHighlight.setPosition(startXSecondLine, startYSecondLine);
    startXSecondLine += textHighlight.getLocalBounds().width;

    textSuffix.setPosition(startXSecondLine, startYSecondLine);

    // Draw the texts
    window.draw(textPrefix);
    window.draw(textHighlight);
    window.draw(textSuffix);
}

bool GraphicsHandler::handleCardSelection(int& selectedCardIndex) {
     bool cardClicked = false;
    // Check if any card in the hand is clicked
    for (int i = 0; i < handSprites.size(); ++i) {
        if (handSprites[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            if (selectedCardIndex == i) {
                // Clicking the selected card again will unselect it
                handSprites[i].setPosition(originalPosition); // Move it back to its original position
                selectedCardIndex = -1;
            } else {
                // Reset selected card to original position
                if (selectedCardIndex != -1) {
                    handSprites[selectedCardIndex].setPosition(originalPosition);
                }
                // Select the card and move it up
                originalPosition = handSprites[i].getPosition();
                handSprites[i].move(0, -MOVE_MOUSE_UPSET); // Move up by moveUpOffset pixels
                selectedCardIndex = i;
            }
            cardClicked = true;
            break;
        }
    }

    // If the click was outside any card and a card was selected, move it back down
    if (!cardClicked && selectedCardIndex != -1) {
        handSprites[selectedCardIndex].setPosition(originalPosition);
        selectedCardIndex = -1;
    }

    return cardClicked;
}

const std::vector<sf::Sprite>& GraphicsHandler::getHandSprites() const {
    return handSprites;
}

const std::vector<vector<sf::Sprite>>& GraphicsHandler::getBoardSprites() const {
    return boardSprites;
} 

const sf::Sprite& GraphicsHandler::getBackgroundSprite() const {
    return backgroundSprite;
}