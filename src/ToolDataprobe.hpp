#ifndef TOOLDATAPROBE_HPP
#define TOOLDATAPROBE_HPP

#include <string>

struct Coordinate 
{
    float x;
    float y;
};

class ToolDataprobe
{
public:
    ToolDataprobe(std::string name="Unnamed dataprobe");
    std::string getName();
    void setName(std::string name);

    Coordinate getPosition();
    void setPosition(Coordinate coordinate);
    void setPosition(float x, float y);

private:
    std::string name;
    Coordinate position;
};

#endif