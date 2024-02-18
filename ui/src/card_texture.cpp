#include "../include/card_texture.h"
#include "../include/assets.h"
#include "../include/globals.h"
#include <iostream>

CardTexture::CardTexture(int number) : number(number) {
    texture = loadCardsTexture(number);
}

const sf::Texture& CardTexture::getTexture() const{
    return texture;
}