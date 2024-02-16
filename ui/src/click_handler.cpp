#include "../include/click_handler.h"

ClickHandler::ClickHandler(sf::RenderWindow& window_, std::vector<sf::Sprite>& handSprites, const std::vector<std::vector<sf::Sprite>>& boardSprites) : window(window_), handSprites(handSprites), boardSprites(boardSprites){
    this->selectedCardIndex = -1;
    this->cardIndexToPlay = -1;
    this->selectedRowIndex = -1;
    this->rowIndexToClean = -1;
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

void ClickHandler::processClick(GraphicsHandler& graphicsHandler, bool waitingForInputToCleanRow){
    if (!waitingForInputToCleanRow){
        graphicsHandler.handleCardSelection(selectedCardIndex);
    } else{
        handleRowSelection(graphicsHandler.getBoardSprites(), window, selectedRowIndex);
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

void ClickHandler::processEnter(){
    chooseCardMove(selectedCardIndex, cardIndexToPlay);
    chooseCleanRowMove(selectedRowIndex, rowIndexToClean);
}


void ClickHandler::reset(){
    this->selectedCardIndex = -1;
    this->cardIndexToPlay = -1;
    this->selectedRowIndex = -1;
    this->rowIndexToClean = -1;
}

void ClickHandler::resetToCleanRowIndex(){
    this->rowIndexToClean = -1;
}

int ClickHandler::getSelectedCardIndex() const {
    return selectedCardIndex;
}

int ClickHandler::getSelectedRowIndex() const{
    return selectedRowIndex;
}

int ClickHandler::getToPlayCardIndex() const{
    return cardIndexToPlay;
}
int ClickHandler::getToCleanRowIndex() const{
    return rowIndexToClean;
}
int& ClickHandler::getToPlayCardIndexRef(){
    return cardIndexToPlay;
}