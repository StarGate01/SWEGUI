#ifndef DATARENDERER_H
#define DATARENDERER_H

#include "../sfml/SFMLWidget.hpp"
#include "NetCdfImageStream.hpp"
#include <glibmm.h>

#define PATH_TO_FRAG_SHADER "ui/fragment.glsl"

namespace renderer
{

  class DataRenderer
  {

    public:

      DataRenderer(sfml::SFMLWidget &widget);

      bool open(std::string filename);

    private:

      sfml::SFMLWidget &widget;

      NetCdfImageStream netcdf_stream;

      sf::RectangleShape background;
      sf::Shader shader;

      //void animate();
      void draw();
      void resize_view();

  };

}

#endif