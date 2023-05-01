#pragma once

#include <SFML/Graphics.hpp>

class Projectile{
    private:
    public:
        float x, y, speed;
        int damage;
        sf::Sprite sprite;
        bool direction;
        Projectile( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite, bool direction);
};