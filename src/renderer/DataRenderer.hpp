#ifndef DATARENDERER_H_INCLUDED
#define DATARENDERER_H_INCLUDED

#include "../sfml/SFMLWidget.hpp"
#include <glibmm.h>

#define PATH_TO_FRAG_SHADER "ui/fragment.glsl"

namespace renderer
{

  class DataRenderer
  {

    public:

      DataRenderer(sfml::SFMLWidget &widget);

    private:

      sfml::SFMLWidget &widget;

      sf::CircleShape circle;
      sf::RectangleShape background;
      sf::Shader shader;
      
      float vx, vy;

      void animate();
      void draw();
      void resize_view();

  };

}

#endif