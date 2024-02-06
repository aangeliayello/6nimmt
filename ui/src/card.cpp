#include "../include/card.h"
#include <iostream>

Card::Card(int number) : number(number) {
    int posCol = (number - 1) % 10;
    int posRow = (number - 1)/10;

    texture.loadFromFile("E:\\Documents\\_FunAndProfit\\6nimmt\\ui\\cards\\cards.jpg", sf::IntRect(132*posCol, 204*posRow, 132, 204));
}

const sf::Texture& Card::getTexture() const{
    return texture;
}