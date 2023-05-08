#include "Door.h"

Door::Door( float xpos, float ypos, sf::Sprite sprite, int destination)
{
    x=xpos;
    y=ypos;
    doorSprite=sprite;
    open = destination;
}

int Door::ChangeLevel()
{
    return open;
}

void Door::setSprite()
{
    doorSprite.setPosition(x, y);
}
