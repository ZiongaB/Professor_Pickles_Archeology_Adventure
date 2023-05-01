#include "Mummy.h"

Mummy::Mummy( float xpos, float ypos,float enemSpeed, int enemDamage, int enemHealth, sf::Sprite sprite, bool direction, int fire)
{
    x=xpos;
    y=ypos;
    speed=enemSpeed;
    damage=enemDamage;
    enemSprite=sprite;
    direction=direction; 
    health = enemHealth;
    fireTime = fire;
}

void Mummy::setSprite()
{
    enemSprite.setPosition(x, y);
}

void Mummy::setDirection()
{
    direction = !direction;
}

void Mummy::increaseTime()
{
    fireTime++;
}