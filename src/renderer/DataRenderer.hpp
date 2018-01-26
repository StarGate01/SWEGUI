#ifndef DATARENDERER_H
#define DATARENDERER_H

#include <gtkmm.h>
#include <glibmm.h>
#include <map>
#include "../widgets/SFMLWidget.hpp"
#include "NetCdfImageStream.hpp"
#include "Layer.hpp"
#include "../probe/DataProbe.hpp"

#define PATH_TO_FRAG_SHADER             "/main/src/ui/main.frag"
#define PATH_TO_FRAG_SCALE_SHADER       "/main/src/ui/scale.frag"
#define PATH_TO_CROSSHAIR_TEX           "/main/src/ui/crosshair.png"
#define PATH_TO_CROSSHAIR_ACTIVE_TEX    "/main/src/ui/crosshair_active.png"
#define PATH_TO_LUT                     "/main/src/ui/lut.png"   
#define PATH_TO_FONT                    "/main/src/ui/courier_prime.ttf"  
#define PATH_TO_GIZMO_TEX               "/main/src/ui/gizmo.png"
#define PATH_TO_ZERO_TEX                "/main/src/ui/zero.png"

#define SCALE_WIDTH 100

namespace renderer
{

  class DataRenderer
  {

    public:

      struct Settings
      {

        bool info = true;
        bool probes = true;
        bool probe_names = true;
        bool probe_indicators = true;
        bool coordinates = true;
        bool zero = true;
        bool gizmo = true;
        bool scales = true;

      };

      NetCdfImageStream::Meta* meta_info = nullptr;

      Layer b, h, hu, hv, hx;
      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };
      map<std::string, probe::DataProbe> probes;
      std::string active_probe_name;
      Settings settings;

      DataRenderer(widgets::SFMLWidget &widget);

      int open(std::string filename);
      int select_timestamp(int timestamp);
      void select_timestamp_async(int timestamp);
      void update_shader();
      void invalidate();
      float sample(NetCdfImageStream::Variable var, float x, float y, int timestamp = -1);
      int get_current_timestamp();
      float get_current_time();

      sf::Vector2f pan = sf::Vector2f(0.f, 0.f);
      float zoom = 1.f;
      void update_transform();

      typedef sigc::signal<void, bool> type_signal_update;
      type_signal_update signal_update();
      typedef sigc::signal<void> type_signal_select;
      type_signal_select signal_select();
      bool save_screenshot(string filename);

    protected:
    
      type_signal_update m_signal_update;
      type_signal_select m_signal_select;

    private:

      struct CoordinateLabel
      {
        float value;
        sf::Vector2f position;
        int orientation;
      };

      widgets::SFMLWidget &widget;
      NetCdfImageStream netcdf_stream;
      sf::RectangleShape background;
      sf::Shader shader, shader_scale;
      sf::Texture crosshair_tex, crosshair_active_tex, lut, gizmo_tex, zero_tex;
      sf::Font font;
      sf::Text info_text, probe_text, coordinate_text, scale_text;
      sf::RectangleShape info_rect, probe_rect, gizmo_rect, zero_rect, scale_rect, scale_bg_rect;
      sf::VertexArray coordinates;
      vector<CoordinateLabel> coordinate_labels;
      int current_timestamp = -1;

      sf::Transform tm_screen_to_tex, tm_screen_to_data, tm_data_to_screen, tm_scale_to_tex;
      sf::Vector2i last_mouse;
      bool pan_active = false;

      void draw();
      void resize_view();
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);
      bool on_button_press_event(GdkEventButton* event);
      bool on_button_release_event(GdkEventButton* event);
      bool on_scroll_event (GdkEventScroll* event);
      bool on_motion_notify_event(GdkEventMotion* event);
      string unique_name();
      void load_texture(std::string path, sf::Texture* tex);
      void update_coordinates();
      string float_to_string(float value);
  };

}

#endif