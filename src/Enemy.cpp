#include "Enemy.h"

Enemy::Enemy( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite)
{
    x=xpos;
    y=ypos;
    speed=enemSpeed;
    damage=enemDamage;
    enemSprite=sprite;
}

void Enemy::setSprite()
{
    enemSprite.setPosition(x, y);
}