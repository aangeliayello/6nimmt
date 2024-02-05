#include <SFML/Graphics.hpp>
#include <memory>
#include "../include/card.h"

// Constants
const float MOVE_MOUSE_UPSET = 20.f; // Pixels to move the card up


void updateHandPositions(std::vector<sf::Sprite>& hand, const sf::RenderWindow& window, float cardWidth, float cardHeight, float horizontalSpacing) {
    // Calculate the total width of all cards and spacing
    float totalWidth = (cardWidth + horizontalSpacing) * hand.size() - horizontalSpacing;
    float startXHand = (window.getSize().x - totalWidth) / 2; // Center horizontally
    float startYHand = window.getSize().y - cardHeight - 20; // Assuming cardHeight is known and 20 pixels padding from bottom

    for (int i = 0; i < hand.size(); ++i) {
        hand[i].setPosition(startXHand + i * (cardWidth + horizontalSpacing), startYHand);
    }
}

void updateBoard(std::vector<std::vector<sf::Sprite>>& board, const sf::RenderWindow& window, int numRows, int cardsPerRow, float horizontalSpacing, float verticalSpacing, float cardWidth, float cardHeight) {
    // Calculate the total width of all cards and spacing
    float startXBoard= horizontalSpacing*5; //(window.getSize().x - totalGridWidth) / 2;
    float startYBoard= verticalSpacing*5; //(window.getSize().y - totalGridHeight) / 2;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            int index = row * board[row].size() + col;
            float x = startXBoard+ col * (cardWidth + horizontalSpacing);
            float y = startYBoard+ row * (cardHeight + verticalSpacing);
            board[row][col].setPosition(x, y);
        }
    }
}

void updateSelectionIndicator(sf::Sprite& selectionIndicator, sf::RenderWindow& window, float horizontalSpacing, float verticalSpacing, float cardHeight, int& selectedRowIndex) {

    if (selectedRowIndex != -1) {
        // Calculate the total width of all cards and spacing
        float startXBoard= horizontalSpacing*5; //(window.getSize().x - totalGridWidth) / 2;
        float startYBoard= verticalSpacing*5; //(window.getSize().y - totalGridHeight) / 2;

        sf::RectangleShape selectionIndicator(sf::Vector2f(horizontalSpacing,  cardHeight*0.8)); // 5 pixels high
        selectionIndicator.setFillColor(sf::Color::Yellow); 

        // Position the circle in front of the row. Adjust x and y as needed.
        float xPosition = startXBoard- horizontalSpacing*2;
        float yPosition = startYBoard+ selectedRowIndex * (cardHeight + verticalSpacing) + 0.1*cardHeight;
        selectionIndicator.setPosition(xPosition, yPosition);

        window.draw(selectionIndicator);
    }
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



int main() {
    const float horizontalSpacing = 10.f; // Space between cards horizontally
    const float verticalSpacing = 10.f; // Space between rows

    // Card selection
    sf::Vector2f originalPosition;


    sf::RenderWindow window(sf::VideoMode(1920u, 1200u), "Zugma");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("E:\\Documents\\_FunAndProfit\\6nimmt\\ui\\cards\\filomena_1 - Copy.jpg")) {
        // Error handling
        return -1; // Or handle the error as you see fit
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    std::vector<std::unique_ptr<Card>> cards;

    // Assuming you have 10 different textures for the cards
    for (int i = 0; i < 104; ++i) {
        cards.push_back(std::make_unique<Card>(i+1));
    }

    const float cardWidth = cards.front()->getTexture().getSize().x;
    const float cardHeight = cards.front()->getTexture().getSize().y;
    // #################
    // Hand 
    // #################
    std::vector<sf::Sprite> hand(10);
    for (int i = 0; i < hand.size(); ++i) {
        hand[i].setTexture(cards[i]->getTexture());
    }
    // Calculate the total width of all cards and spacing
    float totalWidth = (cardWidth + horizontalSpacing) * hand.size() - horizontalSpacing;
    float startXHand = (window.getSize().x - totalWidth) / 2; // Center horizontally
    float startYHand = window.getSize().y - cardHeight - 20; // 20 pixels padding from bottom

    for (int i = 0; i < hand.size(); ++i) {
        hand[i].setPosition(startXHand + i * (cardWidth + horizontalSpacing), startYHand);
    }



    // #############
    // Boad
    // #############
    const int cardsPerRow = 5;
    const int numRows = 4;
    sf::Sprite selectionIndicator;
    std::vector<std::vector<sf::Sprite>> board(numRows, std::vector<sf::Sprite>(cardsPerRow));;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < cardsPerRow; col++) {
            board[row][col].setTexture(cards[(10 + col*5 + row + 1)]->getTexture());
        }
    }

    // Total grid size
    float totalGridWidth = cardWidth * cardsPerRow + horizontalSpacing * (cardsPerRow - 1);
    float totalGridHeight = cardHeight * numRows + verticalSpacing * (numRows - 1);

    // Starting positions to center the grid
    float startXBoard = horizontalSpacing*5; //(window.getSize().x - totalGridWidth) / 2;
    float startYBoard = verticalSpacing*5; //(window.getSize().y - totalGridHeight) / 2;

    // Set positions for each card in the grid
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < cardsPerRow; ++col) {
            int index = row * cardsPerRow + col;
            float x = startXBoard+ col * (cardWidth + horizontalSpacing);
            float y = startYBoard+ row * (cardHeight + verticalSpacing);
            board[row][col].setPosition(x, y);
        }
    }


    int selectedCardIndex = -1;
    int selectedRowIndex = -1;
    bool handSelectedInClick = false;
    bool rowSelectedInClick = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    // Check if a card is selected
                    deleteCardFromHand(hand, board, selectedCardIndex);
                    deleteAllButLastInRow(board, selectedRowIndex);
                    updateBoard(board, window, numRows, cardsPerRow, horizontalSpacing, verticalSpacing, cardWidth, cardHeight); // Ensure cardWidth and horizontalSpacing are defined
                    updateHandPositions(hand, window, cardWidth, cardHeight, horizontalSpacing); // Ensure cardWidth and horizontalSpacing are defined
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handSelectedInClick = handleCardSelection(hand, window, originalPosition, selectedCardIndex);
                    rowSelectedInClick = handleRowSelection(board, window, selectedRowIndex);
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        for (const auto& card : hand) {
            window.draw(card);
        }
        int rowIdx = 0;
        for (const auto& row : board) {
            for (const auto& card : row) {
                window.draw(card);
            }
        }
        updateSelectionIndicator(selectionIndicator,window, horizontalSpacing, verticalSpacing, cardHeight, selectedRowIndex);
        window.display();
    }

    return 0;
}
