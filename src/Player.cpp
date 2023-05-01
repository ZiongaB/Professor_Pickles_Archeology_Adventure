#include "Player.h"

Player::Player( float xpos, float ypos,float playSpeed, int playHealth, sf::Sprite sprite)
{
    x=xpos;
    y=ypos;
    speed=playSpeed;
    health=playHealth;
    playerSprite=sprite;
}

void Player::loseHealth(int enemyDamage)
{
    health -=enemyDamage;
}

void Player::setSprite()
{
    playerSprite.setPosition(x, y);
}
