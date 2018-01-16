#ifndef TOOLDATAPROBE_HPP
#define TOOLDATAPROBE_HPP

#include <string>
#include <SFML/Graphics.hpp>

struct Coordinate 
{
    float x;
    float y;
};

class ToolDataprobe
{

public:

    ToolDataprobe(std::string name = "Unnamed dataprobe");
    std::string getName();
    void setName(std::string n);

    Coordinate getPosition();
    void setPosition(Coordinate coordinate);
    void setPosition(float x, float y);

    sf::Sprite& getSprite();

    bool equals(ToolDataprobe other);

private:

    std::string name;
    Coordinate position;
    sf::Sprite sprite;

};

#endif