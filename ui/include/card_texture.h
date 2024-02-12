#ifndef CARD_TEXTURE_H
#define CARD_TEXTURE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class CardTexture{
    public:
        CardTexture() = default;
        CardTexture(int number);

        const sf::Texture& getTexture() const;

    private:
        int number;
        sf::Texture texture;
};

#endif