#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>

// Textures
sf::Texture loadBackgroundTexture();
sf::Texture loadCardsTexture(int number);

// IconP
sf::Image loadIcon();

// Fonts
sf::Font loadTitleFont();
sf::Font loadTextFont();

#endif