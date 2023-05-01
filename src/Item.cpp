#include "Item.h"
#include <string>

Item::Item(float xpos, float ypos, sf::Sprite sprite, std::string name)
{
    x=xpos;
    y=ypos;
    sprite=sprite;
    name =name;
}

