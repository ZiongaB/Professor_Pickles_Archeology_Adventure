#pragma once

#include <SFML/Graphics.hpp>

class Lion{
    private:
    public:
        float x, y, speed;
        int damage;
        sf::Sprite enemSprite;
        int direction;
        Lion( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite, int direction);
        void setSprite();
        void lowerSpeed();
        void setDirection(int dir);
};