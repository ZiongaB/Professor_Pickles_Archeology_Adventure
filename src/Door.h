#pragma once

#include <SFML/Graphics.hpp>

class Door{
    private:
    public:
        float x, y;
        int open;
        sf::Sprite doorSprite;
        Door( float xpos, float ypos, sf::Sprite sprite, int destination);
        int ChangeLevel();
        void setSprite();

};