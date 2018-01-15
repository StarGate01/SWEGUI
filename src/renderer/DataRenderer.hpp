#ifndef DATARENDERER_H
#define DATARENDERER_H

#include <glibmm.h>
#include "../sfml/SFMLWidget.hpp"
#include "NetCdfImageStream.hpp"
#include "ShaderConstants.hpp"
#include "Layer.hpp"

namespace renderer
{

  class DataRenderer
  {

    public:

      Layer b, h, hu, hv, hx;

      DataRenderer(sfml::SFMLWidget &widget);

      int open(std::string filename);
      int select_timestamp(int timestamp);
      void update_shader();

    private:

      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };

      sfml::SFMLWidget &widget;
      NetCdfImageStream netcdf_stream;
      sf::RectangleShape background;
      sf::Shader shader;

      void draw();
      void resize_view();
      int select_load(NetCdfImageStream::Variable variable, int index, sf::Texture& tex);

  };

}

#endif