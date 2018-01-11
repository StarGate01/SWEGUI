#ifndef MOVINGCIRCLE_H_INCLUDED
#define MOVINGCIRCLE_H_INCLUDED

#include "sfml/SFMLWidget.hpp"
#include <glibmm.h>

class MovingCircle
{

  public:

    MovingCircle(SFMLWidget &widget);

  private:

    SFMLWidget &widget;

    sf::CircleShape circle;
    
    float radius;
    float vx, vy;

    void animate();

    void draw();

    void resize_view();

};

#endif