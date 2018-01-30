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

#define SCALE_WIDTH 200
#define SCALE_HEIGHT 15

/**
 * @brief Map rendering specific classes, such as the custom pixel shader and the DataRenderer class
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
      * @brief Data structure which contains setting for the renderer itself
      */
      struct Settings
      {

        bool info = true;                                                                           ///<Wether the info field should be rendered 
        bool probes = true;                                                                         ///<Wether the probes should be rendered
        bool probe_names = true;                                                                    ///<Wether the name tags of the probes should be rendered
        bool coordinates = true;                                                                    ///<Wether the coordinate markers should be rendered
        bool zero = true;                                                                           ///<Wether the zero marker should be rendered
        bool gizmo = true;                                                                          ///<Wether the axis giszmo should be rendered
        bool scales = true;                                                                         ///<Wether the color scales should be rendered

      };

      NetCdfImageStream::Meta* meta_info = nullptr;                                                 ///<Pointer to the meta info of the lates loaded data block
      Layer b,                                                                                      ///<The bathymetry layer
            h,                                                                                      ///<The water height layer
            hu,                                                                                     ///<The horizontal flux layer
            hv,                                                                                     ///<The vertical flux layer
            hx;                                                                                     ///<The total flux layer
      Layer* layers[5] = { &b, &h, &hu, &hv, &hx };                                                 ///<An array containg pointers to the layers
      map<std::string, probe::DataProbe*> probes;                                                   ///<The map of probes, using their names as indices
      std::string active_probe_name;                                                                ///<The name of the currently selected probe
      Settings settings;                                                                            ///<The Settings of the renderer

      /**
      * @brief Constructor
      *
      * @param &widget The SFML widget which serves as a drawing surface
      */
      DataRenderer(widgets::SFMLWidget &widget);

      void update_shader();                                                                         ///<Updates all shader uniforms
      void invalidate();                                                                            ///<Re-renders the window

      /**
      * @brief Reads a single value from the underlying netcdf data file.
      *
      * @param var Which variable to sample
      * @param x The x position
      * @param y The y position
      * @param timestamp Which timestamp to select
      *
      * @return The sampled value
      */
      float sample(NetCdfImageStream::Variable var, float x, float y, int timestamp = -1);

      /**
       * @brief Gets the currently active timestamp
       * 
       * @return The timestamp
       */
      int get_current_timestamp();

      /**
       * @brief Gets the current time
       * 
       * @return The time which corresponds to the active timestamp
       */
      float get_current_time();

      sf::Vector2f pan = sf::Vector2f(0.f, 0.f);                                                    ///<Panning, in pixels
      float zoom = 1.f;                                                                             ///<Zooming factor
      void update_transform();                                                                      ///<Updates all affine transformation matrices

      typedef sigc::signal<void, bool> type_signal_update;                                          ///<The signal type of the update signal
      type_signal_update signal_update();                                                           ///<The update signal
      typedef sigc::signal<void> type_signal_select;                                                ///<The signal type of the select signal
      type_signal_select signal_select();                                                           ///<The select signal

      /**
      * @brief Saves a screenshot
      *
      * @param filename Where to save the screenshot to
      *
      * @return Wether the operation was succesful
      */
      bool save_screenshot(string filename);

      /**
      * @brief Selects a timestamp in a background task
      *
      * @param timestamp Which timestamp to select
      */
      void select_timestamp_async(int timestamp);
      typedef sigc::signal<void, int> type_signal_done_select_timestamp;                            ///<The signal type of the select timestamp done signal
      type_signal_done_select_timestamp signal_done_select_timestep();                              ///<The select timestamp done signal 

      /**
      * @brief Opens a NetCDF file in a background task
      *
      * @param filename Which file to open
      */
      void open_async(std::string filename);
      typedef sigc::signal<void, int> type_signal_done_open;                                        ///<The signal type of the open done signal
      type_signal_done_open signal_done_open();                                                     ///<The open done signal

      /**
      * @brief Samples a whole batch of values (all timestamps, h, hu, and hv)
      *
      * @param x The x position
      * @param y The y position
      * @param data Where to save the data to
      */
      void sample_batch_async(float x, float y, float**& data);
      typedef sigc::signal<void, int> type_signal_done_sample_batch;                                ///<The signal type of the sample batch done signal
      type_signal_done_sample_batch signal_done_sample_batch();                                     ///<The sample batch done signal

    protected:
    
      type_signal_update m_signal_update;                                                           ///<Internal member of the update signal
      type_signal_select m_signal_select;                                                           ///<Internal member of the select signal
      type_signal_done_select_timestamp m_signal_done_select_timestamp;                             ///<Internal member of the done select timestamp signal
      type_signal_done_open m_signal_done_open;                                                     ///<Internal member of the done open signal
      type_signal_done_sample_batch m_signal_done_sample_batch;                                     ///<Internal member of the done sample batch signal

    private:

      /**
      * @brief This struct represents a label of a coordinate in the coordinate grid
      */
      struct CoordinateLabel
      {
        float value;                                                                                ///<The numeric value
        sf::Vector2f position;                                                                      ///<The pixel position onscreen
        int orientation;                                                                            ///<The orientantion of the label
      };

      widgets::SFMLWidget &widget;                                                                  ///<The underlying SFML widget, which provides the drawing surface
      NetCdfImageStream netcdf_stream;                                                              ///<The underlying data stream, which provides NetCDF data
      sf::RectangleShape background;                                                                ///<The background rectangle, which fills the screen at all times
      sf::Shader  shader,                                                                           ///<The main shader (See: main.frag)
                  shader_scale;                                                                     ///<The scale shader (See: scale.frag)
      sf::Texture crosshair_tex,                                                                    ///<The texture of the crosshair
                  crosshair_active_tex,                                                             ///<The texture of an active crosshair
                  lut,                                                                              ///<The texture of the color lookup map
                  gizmo_tex,                                                                        ///<The texture of the axis gizmo
                  zero_tex;                                                                         ///<The texture of the zero marker
      sf::Font font;                                                                                ///<The font for all text
      sf::Text  info_text,                                                                          ///<The text object for the info field
                probe_text,                                                                         ///<The text object for the probe labels
                coordinate_text,                                                                    ///<The text object for the coordinate numbering
                scale_text;                                                                         ///<The text object for the colro scale labeling
      sf::RectangleShape  info_rect,                                                                ///<The rectangle for the info field
                          probe_rect,                                                               ///<The rectangle for a probe
                          gizmo_rect,                                                               ///<The rectangle for the axis gizmo
                          zero_rect,                                                                ///<The rectangle for the zero marker
                          scale_rect,                                                               ///<The rectangle for a color scale
                          scale_bg_rect;                                                            ///<The rectangle for the background of the color scales
      sf::VertexArray coordinates;                                                                  ///<The vertex array which holds all the lines for the coordinate frame
      vector<CoordinateLabel> coordinate_labels;                                                    ///<A list which holds all The CoordinateLabel s for the coordinate frame
      int current_timestamp = -1;                                                                   ///<The current timestamp

      sf::Transform tm_screen_to_tex,                                                               ///<Affine transformation matrix for screen space to texture space
                    tm_screen_to_data,                                                              ///<Affine transformation matrix for screen space to data space
                    tm_data_to_screen,                                                              ///<Affine transformation matrix for data space to screen space
                    tm_scale_to_tex;                                                                ///<Affine transformation matrix for scale space to texture space
      sf::Vector2i last_mouse;                                                                      ///<The last position of the mouse pointer
      bool pan_active = false;                                                                      ///<Wether panning is currently active

      mutable std::mutex m_stream;                                                                  ///<A mutex lock which protects cuncurrent data access
      std::thread* t_select_timestamp = nullptr,                                                    ///<Thread which handles selecting timestamps
                  *t_open = nullptr,                                                                ///<Thread which handles opening data
                  *t_sample_batch = nullptr;                                                        ///<Thread which handles batch sampling
      Glib::Dispatcher  dispatcher_select_timestamp,                                                ///<Dispatcher for the select timestamp thread
                        dispatcher_open,                                                            ///<Dispatcher for the open thread
                        dispatcher_sample_batch;                                                    ///<Dispatcher for the sample batch thread
      int r_select_timestamp_async = 0,                                                             ///<Result of the select timestamp thread
          r_open_async = 0,                                                                         ///<Result of the open thread
          r_sample_batch = 0;                                                                       ///<Result of the sample batch thread
      void on_thread_select_timestamp_notify();                                                     ///<Handler for notifications from the select timestamp dispatcher
      void on_thread_open_notify();                                                                 ///<Handler for notifications from the open dispatcher
      void on_thread_sample_batch_notify();                                                         ///<Handler for notifications from the sample batch dispatcher

      /**
      * @brief Selects a timestamp synchronous
      *
      * @param timestamp Which timestamp to select
      *
      * @return Wether the operation was successful
      */
      int select_timestamp(int timestamp);

      void draw();                                                                                  ///<Draws the widget
      void resize_view();                                                                           ///<Handler for window resizing events

      /**
      * @brief Loads the data blob of a specific variable
      *
      * @param variable Which variable
      * @param index Which timestamp
      * @param lay Which layer to put the data into
      *
      * @return Wether the operation was successful
      */
      int select_load(NetCdfImageStream::Variable variable, int index, Layer& lay);

      /**
      * @brief Handler for mouse clicks
      *
      * @param event Information about the mouse click
      *
      * @return Wether the event should be swallowed
      */
      bool on_button_press_event(GdkEventButton* event);

      /**
      * @brief Handler for mouse release events
      *
      * @param event Information about the mouse click
      *
      * @return Wether the event should be swallowed
      */
      bool on_button_release_event(GdkEventButton* event);

      /**
      * @brief Handler for mouse scoll events
      *
      * @param event Information about the mouse scoll
      *
      * @return Wether the event should be swallowed
      */
      bool on_scroll_event (GdkEventScroll* event);

      /**
      * @brief Handler for mouse move events
      *
      * @param event nformation about the mouse move
      *
      * @return Wether the event should be swallowed
      */
      bool on_motion_notify_event(GdkEventMotion* event);

      /**
       * @brief Generates a unique probe name
       * 
       * @return The name
       */
      string unique_name();  

      /**
      * @brief Loads a texture
      *
      * @param path The resource path
      *
      * @param tex The texture to load into
      */
      void load_texture(std::string path, sf::Texture* tex);

      /**
       * @brief Updates the vertex buffer for the coordinate frame
       */
      void update_coordinates(); 

      /**
      * @brief Converts a float to a nice string
      *
      * @param value The float
      *
      * @return The string
      */
      string float_to_string(float value);

  };

}

#endif