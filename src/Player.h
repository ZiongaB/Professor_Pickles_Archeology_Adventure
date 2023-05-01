#pragma once

#include <SFML/Graphics.hpp>

class Player
{
    private:
        
    public:
        Player(float xpos, float ypos,float playSpeed, int playHealth, sf::Sprite sprite);
        float x, y, speed;
        int health;
        sf::Sprite playerSprite;
        void loseHealth(int enemyDamage);
        void setSprite ();
};