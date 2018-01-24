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

#define PATH_TO_FONT  "/main/src/ui/courier_prime.ttf"   

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

      bool render_info = true;
      bool render_probes = true;
      bool render_probe_names = true;
      bool render_probe_arrows = true;

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
      sf::Texture crosshair_tex, crosshair_active_tex, lut;
      sf::Font font;
      sf::Text info_text, probe_text;
      sf::RectangleShape info_rect, probe_rect;
      int current_timestamp = -1;

      sf::Transform tm_screen_to_tex, tm_screen_to_data;
      float zoom = 1.f;
      sf::Vector2i last_mouse;
      bool pan_active = false;
      sf::Vector2f pan = sf::Vector2f(0.f, 0.f);

      void draw();
      // void animate();
      void resize_view();
      void update_transform();
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);
      bool on_button_press_event(GdkEventButton* event);
      bool on_button_release_event(GdkEventButton* event);
      bool on_scroll_event (GdkEventScroll* event);
      bool on_motion_notify_event(GdkEventMotion* event);
      string unique_name();

  };

}

#endif