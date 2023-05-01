#pragma once

#include <SFML/Graphics.hpp>

class Door{
    private:
    public:
        float x, y;
        sf::Sprite sprite;
        Door( float xpos, float ypos, sf::Sprite sprite);
};