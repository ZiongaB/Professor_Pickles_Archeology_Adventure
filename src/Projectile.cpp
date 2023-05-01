#include "Projectile.h"

Projectile::Projectile( float xpos, float ypos,float enemSpeed, int enemDamage, sf::Sprite sprite, bool direction)
{
    x=xpos;
    y=ypos;
    speed=enemSpeed;
    damage=enemDamage;
    sprite=sprite;
    direction=direction;
}