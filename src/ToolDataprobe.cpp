#include "ToolDataprobe.hpp"

ToolDataprobe::ToolDataprobe(std::string name)
{
    this->setName(name);
}

std::string ToolDataprobe::getName()
{
    return this->name;
}

void ToolDataprobe::setName(std::string name)
{
    if(name.length() > 0)
        this->name = name;
    else
        this->name = "Invalid name";
}

Coordinate ToolDataprobe::getPosition()
{
    return this->position;
}

void ToolDataprobe::setPosition(Coordinate coordinate)
{
    this->position = coordinate;
}

void ToolDataprobe::setPosition(float x, float y)
{
    Coordinate coordinate = {x, y};
    setPosition(coordinate);
}