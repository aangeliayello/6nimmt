#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Card{
    public:
        Card() = default;
        Card(int number);

        const sf::Texture& getTexture() const;

    private:
        int number;
        sf::Texture texture;
};

#endif