#include "../include/card_texture.h"
#include <iostream>

CardTexture::CardTexture(int number) : number(number) {
    int posCol = (number - 1) % 10;
    int posRow = (number - 1)/10;

    texture.loadFromFile("C:/Users/franc/Documents/_FunAndProfit/6nimmt/ui/cards/cards.jpg", sf::IntRect(132*posCol, 204*posRow, 132, 204));
}

const sf::Texture& CardTexture::getTexture() const{
    return texture;
}