#include "../include/card_texture.h"
#include "../include/assets.h"
#include "../include/globals.h"
#include <iostream>

CardTexture::CardTexture(int number) : number(number) {
    int posCol = (number - 1) % 10;
    int posRow = (number - 1)/10;
    // TODO: Use helper from assets.h
    texture.loadFromFile(BASE_DIR + "assets/cards.jpg", sf::IntRect(132*posCol, 204*posRow, 132, 204));
}

const sf::Texture& CardTexture::getTexture() const{
    return texture;
}