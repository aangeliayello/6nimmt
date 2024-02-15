#include "../include/assets.h"
#include <stdexcept> 
#include "../include/globals.h"

sf::Texture loadBackgroundTexture(){
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(BASE_DIR + "assets/background.jpg")) {
        throw std::runtime_error("Failed to load background texture");
    }
    return backgroundTexture;
}

sf::Texture loadCardsTexture(){
    sf::Texture cardsTexture;
    if (!cardsTexture.loadFromFile(BASE_DIR + "assets/cards.jpg")) {
        throw std::runtime_error("Failed to load cards texture");
    }
    return cardsTexture;
}

sf::Image loadIcon(){
    sf::Image iconImage;
    if (!iconImage.loadFromFile(BASE_DIR + "assets/icon.png")){
        throw std::runtime_error("Failed to load icon image");
    }

    return iconImage;
}

// Fonts
sf::Font loadTitleFont(){
    sf::Font titleFont;
    if (!titleFont.loadFromFile(BASE_DIR + "assets/PressStart2P-Regular.ttf")){
        throw std::runtime_error("Failed to load title font");
    }

    return titleFont;
}
sf::Font loadTextFont(){
    sf::Font textFont;
    if (!textFont.loadFromFile(BASE_DIR + "assets/cour.ttf")){
        throw std::runtime_error("Failed to load text font");
    }

    return textFont;
}

    