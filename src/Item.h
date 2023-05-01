#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Item{
    private:
    public:
        float x, y;
        sf::Sprite sprite;
        std::string name;
        Item(float xpos, float ypos, sf::Sprite sprite, std::string name);
};