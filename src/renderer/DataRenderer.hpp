#ifndef DATARENDERER_H
#define DATARENDERER_H

#include <gtkmm.h>
#include <glibmm.h>
#include <vector>
#include "../sfml/SFMLWidget.hpp"
#include "NetCdfImageStream.hpp"
#include "ShaderConstants.hpp"
#include "Layer.hpp"
#include "../ToolDataprobe.hpp"

namespace renderer
{

  class DataRenderer
  {

    public:

      NetCdfImageStream::Meta* meta_info = nullptr;

      Layer b, h, hu, hv, hx;
      vector<ToolDataprobe> probes;
      ToolDataprobe* active_probe = nullptr;

      DataRenderer(sfml::SFMLWidget &widget);

      int open(std::string filename);
      int select_timestamp(int timestamp);
      void update_shader();
      void invalidate();

      typedef sigc::signal<void, float, float> type_signal_click;
      type_signal_click signal_click();

      typedef sigc::signal<void> type_signal_select;
      type_signal_select signal_select();

    protected:
      type_signal_click m_signal_click;
      type_signal_select m_signal_select;

    private:

      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };

      sfml::SFMLWidget &widget;
      NetCdfImageStream netcdf_stream;
      sf::RectangleShape background;
      sf::Shader shader;

      sf::Texture crosshair_tex, crosshair_active_tex;

      void draw();
      // void animate();
      void resize_view();
      int select_load(NetCdfImageStream::Variable variable, int index, sf::Texture& tex);
      bool on_button_press_event(GdkEventButton *event);

  };

}

#endif