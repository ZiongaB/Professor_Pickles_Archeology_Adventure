#include "Lion.h"

Lion::Lion( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite, int direction)
{
    x=xpos;
    y=ypos;
    speed=enemSpeed;
    damage=enemDamage;
    enemSprite=sprite;
    direction=direction; 
}

void Lion::setSprite()
{
    enemSprite.setPosition(x, y);
}

void Lion::lowerSpeed()
{
    if(speed > 100)
    {
        speed = speed-50;
    }
}

void Lion::setDirection(int dir)
{
    if( dir = 0)
    {
        direction = 1;
    }

    else 
    {
        direction = 0;
    }
}