#include "ToolDataprobe.hpp"

ToolDataprobe::ToolDataprobe(std::string name)
{
    setName(name);
    sprite.setOrigin(16.f, 16.f);
}

std::string ToolDataprobe::getName()
{
    return name;
}

void ToolDataprobe::setName(std::string n)
{
    if(n.length() > 0) name = n;
    else name = "Invalid name";
}

Coordinate ToolDataprobe::getPosition()
{
    return position;
}

void ToolDataprobe::setPosition(Coordinate coordinate)
{
    position = coordinate;
}

void ToolDataprobe::setPosition(float x, float y)
{
    Coordinate coordinate = {x, y};
    setPosition(coordinate);
}

sf::Sprite& ToolDataprobe::getSprite()
{
    return sprite;
}

bool ToolDataprobe::equals(ToolDataprobe other)
{
    Coordinate c = other.getPosition();
    if(c.x != position.x || c.y != position.y)
        return false;
    return this->getName().compare(other.getName()) == 0;
}