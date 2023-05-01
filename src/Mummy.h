#pragma once

#include <SFML/Graphics.hpp>

class Mummy{
    private:
    public:
        float x, y, speed;
        int damage;
        sf::Sprite enemSprite;
        bool direction;
        int health;
        int fireTime;

        Mummy( float xpos, float ypos,float enemSpeed, int enemDamage, int enemHealth, sf::Sprite sprite, bool direction, int fire);
        void setSprite();
        void setDirection();
        void increaseTime();
};