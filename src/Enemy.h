#pragma once

#include <SFML/Graphics.hpp>

class Enemy{
    private:
    public:
        float x, y, speed;
        int damage;
        sf::Sprite enemSprite;
        Enemy( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite);
        void setSprite();
};