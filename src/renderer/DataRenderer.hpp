#ifndef DATARENDERER_H
#define DATARENDERER_H

#include <gtkmm.h>
#include <glibmm.h>
#include <map>
#include "../widgets/SFMLWidget.hpp"
#include "NetCdfImageStream.hpp"
#include "ShaderConstants.hpp"
#include "Layer.hpp"
#include "../probe/DataProbe.hpp"

namespace renderer
{

  class DataRenderer
  {

    public:

      NetCdfImageStream::Meta* meta_info = nullptr;

      Layer b, h, hu, hv, hx;
      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };
      map<std::string, probe::DataProbe> probes;
      std::string active_probe_name;

      DataRenderer(widgets::SFMLWidget &widget);

      int open(std::string filename);
      int select_timestamp(int timestamp);
      void update_shader();
      void invalidate();

      typedef sigc::signal<void, bool> type_signal_update;
      type_signal_update signal_update();
      typedef sigc::signal<void> type_signal_select;
      type_signal_select signal_select();

    protected:
    
      type_signal_update m_signal_update;
      type_signal_select m_signal_select;

    private:

      widgets::SFMLWidget &widget;
      NetCdfImageStream netcdf_stream;
      sf::RectangleShape background;
      sf::Shader shader;
      float pad_v = 0.f, pad_h = 0.f;

      sf::Texture crosshair_tex, crosshair_active_tex;

      void draw();
      // void animate();
      void resize_view();
      void update_padding();
      sf::Vector2f screen_to_data(sf::Vector2f coord);
      sf::Vector2f data_to_screen(sf::Vector2f coord);
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);
      bool on_button_press_event(GdkEventButton *event);
      string unique_name();

  };

}

#endif