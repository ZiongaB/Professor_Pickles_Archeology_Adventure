#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Item{
    private:
    public:
        float x, y;
        sf::Sprite itemSprite;
        std::string itemValue;
        Item(float xpos, float ypos, sf::Sprite sprite, std::string name);
        std::string AddItem();
        void setSprite();

};