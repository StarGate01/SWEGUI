#ifndef MOVINGCIRCLE_H_INCLUDED
#define MOVINGCIRCLE_H_INCLUDED

#include "sfml/SFMLWidget.hpp"
#include <glibmm.h>

#define PATH_TO_FRAG_SHADER "ui/fragment.glsl"

class MovingCircle
{

  public:

    MovingCircle(SFMLWidget &widget);

  private:

    SFMLWidget &widget;

    sf::CircleShape circle;
    sf::RectangleShape background;
    sf::Shader shader;
    
    float vx, vy;

    void animate();
    void draw();
    void resize_view();

};

#endif