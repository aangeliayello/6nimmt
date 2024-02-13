#include <SFML/Graphics.hpp>
#include <memory>
#include "../include/card_texture.h"
#include "../../bots/include/core/game.h"
#include "../../bots/include/game_factory.h"

// Constants
float BASE_WIDTH = 1920.0f; // Assume this is your game's designed resolution width
float BASE_HEIGHT = 1200.0f; // And this is the height
float SCALE_FACTOR;

const float MOVE_MOUSE_UPSET = 15.f; // Pixels to move the card up
const float HORIZONTAL_SPACING = 20.f; // Space between cards horizontally
const float VERTICAL_SPACING = 15.f; 

void setHand(vector<sf::Sprite>& hand, vector<int>& handValue, vector<unique_ptr<CardTexture>>& cards) {
    hand.clear();
    hand.resize(handValue.size());
    for (int i = 0; i < handValue.size(); ++i) {
        int cardIdx = handValue[i]-1;
        hand[i].setTexture(cards[cardIdx]->getTexture());
        hand[i].setScale(SCALE_FACTOR, SCALE_FACTOR);
    }
}

void setBoard(vector<vector<sf::Sprite>>& board, vector<vector<int>>& boardValues, vector<unique_ptr<CardTexture>>& cards) {
    board.clear();
    board.resize(boardValues.size());
    for (int row = 0; row < boardValues.size(); row++) {
        board[row].reserve(boardValues[row].size());
        for (int col = 0; col < boardValues[row].size(); col++) {
            int cardIdx = boardValues[row][col]-1;
            sf::Sprite sprite;
            sprite.setTexture(cards[cardIdx]->getTexture());
            sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);

            board[row].push_back(std::move(sprite));
        }
    }
}

void drawOutline(const sf::Sprite& card, sf::RenderWindow& window, sf::Color color = sf::Color::Black){
    float thickness = 2;
    sf::Vector2f cardSize(card.getGlobalBounds().width, card.getGlobalBounds().height);
    sf::Vector2f outlineSize(cardSize.x + 2* thickness, cardSize.y + 2*thickness);
    sf::RectangleShape cardOutline(outlineSize);

    cardOutline.setPosition(card.getPosition().x - thickness, card.getPosition().y - thickness);
    cardOutline.setFillColor(color); // Or any color for the outline
    cardOutline.setOutlineThickness(0);

    window.draw(cardOutline);
}

void updateHandPositions(std::vector<sf::Sprite>& hand, const Game& game, vector<unique_ptr<CardTexture>>& cards, const sf::RenderWindow& window, float cardWidth, float cardHeight) {
    vector<int> handValues = getHand(game);
    setHand(hand, handValues, cards);
    
    // Calculate the total width of all cards and spacing
    float horizontalSpacing = HORIZONTAL_SPACING*SCALE_FACTOR;
    float verticalSpacing = VERTICAL_SPACING*SCALE_FACTOR;
    float totalWidth = (cardWidth + horizontalSpacing) * hand.size() - horizontalSpacing;
    float startXHand = (window.getSize().x - totalWidth) / 2; // Center horizontally
    float startYHand = window.getSize().y - cardHeight - verticalSpacing; // Assuming cardHeight is known and 20 pixels padding from bottom

    for (int i = 0; i < hand.size(); ++i) {
        hand[i].setPosition(startXHand + i * (cardWidth + horizontalSpacing), startYHand);
    }
}

void updateBoard(std::vector<std::vector<sf::Sprite>>& board, const Game& game, vector<unique_ptr<CardTexture>>& cards, const sf::RenderWindow& window, int numRows, int cardsPerRow, float cardWidth, float cardHeight) {
    
    auto boardValues = getBoard(game);
    setBoard(board, boardValues, cards);

    // Calculate the total width of all cards and spacing
    float horizontalSpacing  = HORIZONTAL_SPACING*SCALE_FACTOR;
    float verticalSpacing = VERTICAL_SPACING*SCALE_FACTOR;
    float startXBoard= horizontalSpacing*2; //(window.getSize().x - totalGridWidth) / 2;
    float startYBoard= verticalSpacing*2; //(window.getSize().y - totalGridHeight) / 2;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            int index = row * board[row].size() + col;
            float x = startXBoard+ col * (cardWidth + horizontalSpacing);
            float y = startYBoard+ row * (cardHeight + verticalSpacing);
            board[row][col].setPosition(x, y);
        }
    }
}

void updateShownCards(std::vector<sf::Sprite>& showCards, const Game& game, vector<unique_ptr<CardTexture>>& cards, sf::RenderWindow& window, int cardsPerRow, float cardWidth, float cardHeight, sf::Font font) {
    
    vector<int> cardsToShow = getShownCards(game);
    setHand(showCards, cardsToShow, cards);

    // Calculate the total width of all cards and spacing
    float horizontalSpacing  = HORIZONTAL_SPACING*SCALE_FACTOR;
    float verticalSpacing = VERTICAL_SPACING*SCALE_FACTOR;
    float startYBoard= verticalSpacing*5; //(window.getSize().y - totalGridHeight) / 2;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14); // Adjust size as needed
    text.setFillColor(sf::Color::White); // Inner color
    text.setOutlineColor(sf::Color::Black); // Border color
    text.setOutlineThickness(2); // Adjust thickness as needed
    text.setStyle(sf::Text::Bold); // Set text style
    text.setString("Players Cards:");
    
    sf::FloatRect textRect = text.getLocalBounds();
    float startXBoard= window.getSize().x - 5*horizontalSpacing - max(cardsToShow.size()*(cardWidth + horizontalSpacing), textRect.getSize().x); //(window.getSize().x - totalGridWidth) / 2;
    float startYCards = startYBoard + verticalSpacing + textRect.getSize().y; 

    text.setPosition(sf::Vector2f(startXBoard, startYBoard));
    float x;
    for (int col = 0; col < cardsToShow.size(); ++col) {
        x = startXBoard+ col * (cardWidth + horizontalSpacing);
        showCards[col].setPosition(x, startYCards);
    }
    
    const auto& players = game.getPlayers();

    float labelYPosition = startYCards + (cardHeight + verticalSpacing);
    vector<sf::Text> playerNamesLabels;
    int col = 0;
    for (const auto& player : players){
        sf::Text label;
        label.setFont(font);
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
    for (const auto& card : showCards){
        drawOutline(card, window);
        window.draw(card);
    }

    // Player labes
    for (auto& label : playerNamesLabels){
        window.draw(label);
    }

    // Title text
    window.draw(text);
}

void updateSelectionIndicator(sf::Sprite& selectionIndicator, sf::RenderWindow& window, const Board& board, float cardWidth, float cardHeight, int& selectedRowIndex) {

    if (selectedRowIndex != -1) {
        float horizontalSpacing  = HORIZONTAL_SPACING*SCALE_FACTOR;
        float verticalSpacing = VERTICAL_SPACING*SCALE_FACTOR;
        // Calculate the total width of all cards and spacing
        float startXBoard= horizontalSpacing*2; //(window.getSize().x - totalGridWidth) / 2;
        float startYBoard= verticalSpacing*2; //(window.getSize().y - totalGridHeight) / 2;

        sf::RectangleShape selectionIndicatorFront(sf::Vector2f(horizontalSpacing/2,  cardHeight*0.8)); // 5 pixels high
        selectionIndicatorFront.setFillColor(sf::Color::Red); 

        // Position the circle in front of the row. Adjust x and y as needed.
        float xPosition = startXBoard - horizontalSpacing*1.5;
        float yPosition = startYBoard+ selectedRowIndex * (cardHeight + verticalSpacing) + 0.1*cardHeight;
        selectionIndicatorFront.setPosition(xPosition, yPosition);

        sf::RectangleShape selectionIndicatorEnd(sf::Vector2f(horizontalSpacing/2,  cardHeight*0.8)); // 5 pixels high
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

void updateScoreBoard(vector<int> scores, const vector<unique_ptr<Player>>& players, sf::RenderWindow& window, const sf::Font font) {
    string scoreText = "Scores:\n";
    for (int i  = 0; i < scores.size(); i++){
        scoreText += players[i]->getName() + ": " + to_string(scores[i]) + "\n";
    }

    // Calculate the total width of all cards and spacing
    float horizontalSpacing = HORIZONTAL_SPACING * SCALE_FACTOR;
    float verticalSpacing = VERTICAL_SPACING * SCALE_FACTOR;

    sf::Text text;
    text.setFont(font);
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

void updateExpectedMoveMessage1(bool waitingForInputToPlayCard, bool waitingForInputToCleanRow, sf::RenderWindow& window, const sf::Font& font, float cardWidth, float cardHeight) {
    // Calculate the total width of all cards and spacing
    float horizontalSpacing = HORIZONTAL_SPACING * SCALE_FACTOR;
    float verticalSpacing = VERTICAL_SPACING * SCALE_FACTOR;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14); // Adjust size as needed
    text.setFillColor(sf::Color::White); // Inner color
    text.setOutlineColor(sf::Color::Black); // Border color
    text.setOutlineThickness(2); // Adjust thickness as needed
    text.setStyle(sf::Text::Bold); // Set text style

    if (waitingForInputToPlayCard) {
        // Display message that player needs to select a card and press enter
        text.setString("Select a CARD and press enter.");
    } else if (waitingForInputToCleanRow) {
        // Display message that player needs to select a row and press enter
        text.setString("Select a ROW and press enter.");
    }

    // Calculate the text's center position
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,0);
    text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y - cardHeight - verticalSpacing * 3 - textRect.height));

    window.draw(text);
}

void updateExpectedMoveMessage(bool waitingForInputToPlayCard, bool waitingForInputToCleanRow, sf::RenderWindow& window, const sf::Font& font, float cardWidth, float cardHeight) {
    if (waitingForInputToPlayCard || waitingForInputToCleanRow){

        // Common settings
        float verticalSpacing = VERTICAL_SPACING * SCALE_FACTOR;
        unsigned int characterSize = 14; // Adjust size as needed
        float outlineThickness = 2; // Adjust thickness as needed

        // Prepare texts
        sf::Text textPrefix;
        textPrefix.setFont(font);
        textPrefix.setString("Select a ");
        textPrefix.setCharacterSize(characterSize);
        textPrefix.setFillColor(sf::Color::White);
        textPrefix.setOutlineColor(sf::Color::Black);
        textPrefix.setOutlineThickness(outlineThickness);
        textPrefix.setStyle(sf::Text::Bold);

        sf::Text textHighlight;
        textHighlight.setFont(font);
        textHighlight.setString(waitingForInputToPlayCard ? "CARD" : "ROW");
        textHighlight.setCharacterSize(characterSize);
        textHighlight.setFillColor(waitingForInputToPlayCard ? sf::Color::Red : sf::Color::Blue); // Red for highlight
        textHighlight.setOutlineColor(sf::Color::Black);
        textHighlight.setOutlineThickness(outlineThickness);
        textHighlight.setStyle(sf::Text::Bold);

        sf::Text textSuffix;
        textSuffix.setFont(font);
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

void updateGameOverMsg(string winner, sf::RenderWindow& window, const sf::Font& font) {

    // Common settings
    float verticalSpacing = VERTICAL_SPACING * SCALE_FACTOR;
    unsigned int characterSize = 24; // Adjust size as needed
    float outlineThickness = 3; // Adjust thickness as needed

    // Prepare texts
    sf::Text textPrefix;
    textPrefix.setFont(font);
    textPrefix.setString("GAME OVER!\n");

    textPrefix.setCharacterSize(characterSize);
    textPrefix.setFillColor(sf::Color::Cyan);
    textPrefix.setOutlineColor(sf::Color::Black);
    textPrefix.setOutlineThickness(outlineThickness);
    textPrefix.setStyle(sf::Text::Bold);

    sf::Text textHighlight;
    textHighlight.setFont(font);
    textHighlight.setString(winner);
    textHighlight.setCharacterSize(characterSize);
    textHighlight.setFillColor(sf::Color::Magenta); // Red for highlight
    textHighlight.setOutlineColor(sf::Color::Black);
    textHighlight.setOutlineThickness(outlineThickness);
    textHighlight.setStyle(sf::Text::Bold);

    sf::Text textSuffix;
    textSuffix.setFont(font);
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

bool handleCardSelection(std::vector<sf::Sprite>& hand, const sf::RenderWindow& window, sf::Vector2f& originalPosition, int& selectedCardIndex) {
     bool cardClicked = false;
    // Check if any card in the hand is clicked
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            if (selectedCardIndex == i) {
                // Clicking the selected card again will unselect it
                hand[i].setPosition(originalPosition); // Move it back to its original position
                selectedCardIndex = -1;
            } else {
                // Reset selected card to original position
                if (selectedCardIndex != -1) {
                    hand[selectedCardIndex].setPosition(originalPosition);
                }
                // Select the card and move it up
                originalPosition = hand[i].getPosition();
                hand[i].move(0, -MOVE_MOUSE_UPSET); // Move up by moveUpOffset pixels
                selectedCardIndex = i;
            }
            cardClicked = true;
            break;
        }
    }
    // If the click was outside any card and a card was selected, move it back down
    if (!cardClicked && selectedCardIndex != -1) {
        hand[selectedCardIndex].setPosition(originalPosition);
        selectedCardIndex = -1;
    }

    return cardClicked;
}
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

    sf::Font font;
    if (!font.loadFromFile("C:/Users/franc/Documents/_FunAndProfit/6nimmt/ui/ca/PressStart2P-Regular.ttf")) {
        return EXIT_FAILURE;
    }

    sf::Font fontCourier;
    if (!fontCourier.loadFromFile("C:/Users/franc/Documents/_FunAndProfit/6nimmt/ui/ca/cour.ttf")) {
        return EXIT_FAILURE;
    }

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.width;
    unsigned int screenHeight = desktop.height;
    sf::RenderWindow window(sf::VideoMode(screenWidth*0.4, screenHeight*0.4), "Zugma");
    sf::Image icon;
    icon.loadFromFile("C:/Users/franc/Documents/_FunAndProfit/6nimmt/ui/cards/icon.png"); // File/Image/Pixel
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    // Calculate scaling factors based on the screen size versus some base size
    float currentWidth = window.getSize().x;
    float currentHeight = window.getSize().y;
    float scaleX = currentWidth / BASE_WIDTH;
    float scaleY = currentHeight / BASE_HEIGHT;
    SCALE_FACTOR = min(scaleX, scaleY);
    
    // Backend
    Game game = Game();
    startGame(game);
    vector<vector<int>> boardValues = getBoard(game);
    vector<int> handValue = getHand(game);

    // Card selection
    sf::Vector2f originalPosition;


    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/franc/Documents/_FunAndProfit/6nimmt/ui/cards/filomena_1 - Copy.jpg")) {
        // Error handling
        return -1; // Or handle the error as you see fit
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    backgroundSprite.setScale(scaleX, scaleY);
    std::vector<std::unique_ptr<CardTexture>> cards;

    // Assuming you have 10 different textures for the cards
    for (int i = 0; i < 104; ++i) {
        cards.push_back(std::make_unique<CardTexture>(i+1));
    }

    // #################
    // Hand 
    // #################
    std::vector<sf::Sprite> shownCards;
    std::vector<sf::Sprite> hand;
    setHand(hand, handValue, cards);

    const float originalCardWidth = hand[0].getTexture()->getSize().x;
    const float originalCardHeight = hand[0].getTexture()->getSize().y;

    float cardWidth = originalCardWidth * SCALE_FACTOR;
    float cardHeight = originalCardHeight * SCALE_FACTOR;

    updateHandPositions(hand, game, cards, window, cardWidth, cardHeight);

    // #############
    // Boad
    // #############
    const int cardsPerRow = 5;
    const int numRows = 4;
    sf::Sprite selectionIndicator;
    
    std::vector<std::vector<sf::Sprite>> board;
    setBoard(board, boardValues, cards);

    // Total grid size
    float boardHorizontalSpacing = HORIZONTAL_SPACING*SCALE_FACTOR;
    float boardVerticalSpacing = VERTICAL_SPACING*SCALE_FACTOR;
    float totalGridWidth = cardWidth * cardsPerRow + boardHorizontalSpacing * (cardsPerRow - 1);
    float totalGridHeight = cardHeight * numRows + boardVerticalSpacing * (numRows - 1);

    // Starting positions to center the grid
    float startXBoard = boardHorizontalSpacing*2; //(window.getSize().x - totalGridWidth) / 2;
    float startYBoard = boardVerticalSpacing*2; //(window.getSize().y - totalGridHeight) / 2;

    // Set positions for each card in the grid
    updateBoard(board, game, cards, window, numRows, cardsPerRow, cardWidth, cardHeight);

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

                // Calculate scaling factors based on the screen size versus some base size
                float currentWidth = window.getSize().x;
                float currentHeight = window.getSize().y;
                float scaleX = currentWidth / BASE_WIDTH;
                float scaleY = currentHeight / BASE_HEIGHT;
                SCALE_FACTOR = std::min(scaleX, scaleY); 

                backgroundSprite.setScale(scaleX, scaleY);

                cardWidth = originalCardWidth * SCALE_FACTOR;
                cardHeight = originalCardHeight * SCALE_FACTOR;

                updateHandPositions(hand, game, cards, window, cardWidth, cardHeight);
                updateBoard(board, game, cards, window, numRows, cardsPerRow, cardWidth, cardHeight);
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
                updateHandPositions(hand, game, cards, window, cardWidth, cardHeight);

                // Show players cards TODO
                bool todo = game.checkIfInputToCleanRowNeeded();
                if (!todo){
                    game.processMoves();
                    updateBoard(board, game, cards, window, numRows, cardsPerRow, cardWidth, cardHeight);
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
                    updateBoard(board, game, cards, window, numRows, cardsPerRow, cardWidth, cardHeight);
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
                    updateHandPositions(hand, game, cards, window, cardWidth, cardHeight);
                    updateBoard(board, game, cards, window, numRows, cardsPerRow, cardWidth, cardHeight);
                    break;
                }  
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!waitingForInputToCleanRow){
                        handSelectedInClick = handleCardSelection(hand, window, originalPosition, selectedCardIndex);
                    } else{
                        rowSelectedInClick = handleRowSelection(board, window, selectedRowIndex);
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        int cardIdx = 0;
        sf::Color color;
        for (const auto& card : hand) {
            if (cardIdx == selectedCardIndex){
                color = sf::Color::Red;
            } else{
                color = sf::Color::Black;
            }
            drawOutline(card, window, color);
            window.draw(card);
            cardIdx++;
        }

        int rowIdx = 0;
        for (const auto& row : board) {
            for (const auto& card : row) {
                drawOutline(card, window);
                window.draw(card);
            }
        }
        if (waitingForInputToCleanRow){
            updateShownCards(shownCards, game, cards, window, cardsPerRow, cardWidth, cardHeight, font);
        }
        updateExpectedMoveMessage(waitingForInputToPlayCard, waitingForInputToCleanRow, window, font, cardWidth, cardHeight);
        updateScoreBoard(game.getScores(), game.getPlayers(), window, fontCourier);
        updateSelectionIndicator(selectionIndicator,window, game.getBoard(), cardWidth, cardHeight, selectedRowIndex);
        if (gameOver){
            updateGameOverMsg(game.getWinner(), window, font);
        }
        window.display();
    }

    return 0;
}
