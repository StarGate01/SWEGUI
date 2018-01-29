/**
 * @brief Renderer for the simulation data in the main gui window
 * @file DataRenderer.hpp
*/

#ifndef DATARENDERER_H
#define DATARENDERER_H

#include <gtkmm.h>
#include <glibmm.h>
#include <mutex>
#include <thread>
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


/**
 * @brief Map rendering specific classes, such as the custom pixelshader and the DataRenderer class
*/
namespace renderer
{
  /**
  * @brief Renderer for the simulation data in the main gui window
  */
  class DataRenderer
  {

    public:
      /**
      * @brief 
      */
      struct Settings
      {

        bool info = true;                                                                           ///<
        bool probes = true;                                                                         ///<
        bool probe_names = true;                                                                    ///<
        bool probe_indicators = true;                                                               ///<
        bool coordinates = true;                                                                    ///<
        bool zero = true;                                                                           ///<
        bool gizmo = true;                                                                          ///<
        bool scales = true;                                                                         ///<

      };

      NetCdfImageStream::Meta* meta_info = nullptr;                                                 ///<

      Layer b, h, hu, hv, hx;                                                                       ///<
      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };                                                 ///<
      map<std::string, probe::DataProbe> probes;                                                    ///<
      std::string active_probe_name;                                                                ///<
      Settings settings;                                                                            ///<

      /**
      * @brief 
      * @param &widget
      */
      DataRenderer(widgets::SFMLWidget &widget);

      void update_shader();                                                                         ///<
      void invalidate();                                                                            ///<

      /**
      * @brief 
      * @param var
      * @param x
      * @param y
      * @param timestamp
      * @param lock
      * @return
      */
      float sample(NetCdfImageStream::Variable var, float x, float y, int timestamp = -1, bool lock = true);
      int get_current_timestamp();                                                                  ///<
      float get_current_time();                                                                     ///<

      sf::Vector2f pan = sf::Vector2f(0.f, 0.f);                                                    ///<
      float zoom = 1.f;                                                                             ///<
      void update_transform();                                                                      ///<

      typedef sigc::signal<void, bool> type_signal_update;                                          ///<
      type_signal_update signal_update();                                                           ///<
      typedef sigc::signal<void> type_signal_select;                                                ///<
      type_signal_select signal_select();                                                           ///<

      /**
      * @brief 
      * @param filename
      * @return
      */
      bool save_screenshot(string filename);

      /**
      * @brief 
      * @param timestamp
      */
      void select_timestamp_async(int timestamp);
      typedef sigc::signal<void, int> type_signal_done_select_timestamp;                            ///<
      type_signal_done_select_timestamp signal_done_select_timestep();                              ///<

      /**
      * @brief 
      * @param filename
      */
      void open_async(std::string filename);
      typedef sigc::signal<void, int> type_signal_done_open;                                        ///<
      type_signal_done_open signal_done_open();                                                     ///<

      /**
      * @brief 
      * @param x
      * @param y
      * @param data
      */
      void sample_batch_async(float x, float y, float**& data);
      typedef sigc::signal<void, int> type_signal_done_sample_batch;                                ///<
      type_signal_done_sample_batch signal_done_sample_batch();                                     ///<

    protected:
    
      type_signal_update m_signal_update;                                                           ///<
      type_signal_select m_signal_select;                                                           ///<
      type_signal_done_select_timestamp m_signal_done_select_timestamp;                             ///<
      type_signal_done_open m_signal_done_open;                                                     ///<
      type_signal_done_sample_batch m_signal_done_sample_batch;                                     ///<

    private:
      /**
      * @brief 
      */
      struct CoordinateLabel
      {
        float value;                                                                                ///<
        sf::Vector2f position;                                                                      ///<
        int orientation;                                                                            ///<
      };

      widgets::SFMLWidget &widget;                                                                  ///<
      NetCdfImageStream netcdf_stream;                                                              ///<
      sf::RectangleShape background;                                                                ///<
      sf::Shader shader, shader_scale;                                                              ///<
      sf::Texture crosshair_tex, crosshair_active_tex, lut, gizmo_tex, zero_tex;                    ///<
      sf::Font font;                                                                                ///<
      sf::Text info_text, probe_text, coordinate_text, scale_text;                                  ///<
      sf::RectangleShape info_rect, probe_rect, gizmo_rect, zero_rect, scale_rect, scale_bg_rect;   ///<
      sf::VertexArray coordinates;                                                                  ///<
      vector<CoordinateLabel> coordinate_labels;                                                    ///<
      int current_timestamp = -1;                                                                   ///<

      sf::Transform tm_screen_to_tex, tm_screen_to_data, tm_data_to_screen, tm_scale_to_tex;        ///<
      sf::Vector2i last_mouse;                                                                      ///<
      bool pan_active = false;                                                                      ///<

      mutable std::mutex m_stream;                                                                  ///<
      std::thread* t_select_timestamp = nullptr, *t_open = nullptr, *t_sample_batch = nullptr;      ///<
      Glib::Dispatcher dispatcher_select_timestamp, dispatcher_open, dispatcher_sample_batch;       ///<
      int r_select_timestamp_async = 0, r_open_async = 0, r_sample_batch = 0;                       ///<
      void on_thread_select_timestamp_notify();                                                     ///<
      void on_thread_open_notify();                                                                 ///<
      void on_thread_sample_batch_notify();                                                         ///<
      /**
      * @brief 
      * @param timestamp
      * @return
      */
      int select_timestamp(int timestamp);

      void draw();                                                                                  ///<
      void resize_view();                                                                           ///<
      /**
      * @brief 
      * @param variable
      * @param index
      * @param lay
      */
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);
      /**
      * @brief 
      * @param event
      * @return
      */
      bool on_button_press_event(GdkEventButton* event);
      /**
      * @brief 
      * @param event
      * @return
      */
      bool on_button_release_event(GdkEventButton* event);
      /**
      * @brief 
      * @param event
      * @return
      */
      bool on_scroll_event (GdkEventScroll* event);
      /**
      * @brief 
      * @param event
      * @return
      */
      bool on_motion_notify_event(GdkEventMotion* event);
      string unique_name();                                                                         ///<
      /**
      * @brief 
      * @param path
      * @param tex
      */
      void load_texture(std::string path, sf::Texture* tex);
      void update_coordinates();                                                                    ///<
      /**
      * @brief 
      * @param value
      * @return
      */
      string float_to_string(float value);
  };

}

#endif