#include "Item.h"
#include <string>

Item::Item(float xpos, float ypos, sf::Sprite sprite, std::string name)
{
    x=xpos;
    y=ypos;
    itemSprite=sprite;
    itemValue =name;
}

std::string Item::AddItem()
{
    return itemValue;
}

void Item::setSprite()
{
    itemSprite.setPosition(x, y);
}

