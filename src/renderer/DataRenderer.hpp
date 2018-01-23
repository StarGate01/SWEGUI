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
      float sample(NetCdfImageStream::Variable var, float x, float y, int timestamp = -1);
      int get_current_timestamp();
      float get_current_time();

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

      int current_timestamp = -1;
      sf::Transform screen_to_tex, tex_to_data;
      float zoom = 1.f, pan_x = 0.f, pan_y = 0.f;

      sf::Texture crosshair_tex, crosshair_active_tex, lut;

      void draw();
      // void animate();
      void resize_view();
      void update_transform();
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);
      bool on_button_press_event(GdkEventButton *event);
      bool on_scroll_event (GdkEventScroll *event);
      string unique_name();

  };

}

#endif