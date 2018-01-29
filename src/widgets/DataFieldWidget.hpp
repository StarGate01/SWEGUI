/**
 * @file DataFieldWidget.hpp
 * @brief Custom widget to visualize probe data as raw data and as graph
*/

#ifndef DATAFIELD_WIDGET_H
#define DATAFIELD_WIDGET_H

#include <string>
#include <gtkmm.h>
#include <gtkmm/messagedialog.h>
#include <pangomm/attrlist.h>   //Set attributes for labels
#include <cairomm/context.h>

#define PATH_TO_DATAFIELD_GUI "/main/src/ui/datafield.glade"        ///<Path to Glade file of the widget

#define GRAPH_SCALE 0.7                                             ///<Ratio maximal graph data height to graph window height
#define COLOR_WATER "#005ce6"                                       ///<Hex color for water
#define COLOR_SPEED1 "#bbff99"                                      ///<Hex color for speed visualisation (1)
#define COLOR_SPEED2 "#aaff80"                                      ///<Hex color for speed visualisation (2)
#define COLOR_SPEED3 "#99ff66"                                      ///<Hex color for speed visualisation (3)
#define COLOR_DEFAULT "#d3d3d3"                                     ///<Default hex color
#define LEGEND_FONT_SIZE 10                                         ///<Font size of the min and max values
#define LEGEND_H_DEVISION 6                                         ///<Maximum horizonal number of reference lines
#define LEGEND_V_MAX_DEVISION 10                                    ///<Vertical number of reference lines
#define MIN_OFFSET 0.15                                             ///<Offset between bottom of the graph and its minimal value

namespace swegui
{

  class MainWindow;

}

/**
 * @brief Namespace for custom GTK widgets
*/
namespace widgets
{

  /**
   * @brief Custom widget to visualize probe data as raw data and as graph
  */
  class DataFieldWidget : public Gtk::Notebook
  {

    public:
      /**
       * @brief Inherited constructor of Gtk::Notebook
       * @param cobject Calling object
       * @param refBuilder Gtk::Builder object for this widget
       * @return New instance of widgets::DataFieldWidget
      */
      DataFieldWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);

      /**
       * @brief Creates a new instance of widgets::DataFieldWidget
       * @param pa Parent swegui::MainWindow
       * @param na Intended name for this widget
      */
      static DataFieldWidget* create(swegui::MainWindow* pa, std::string na);

      void update_ui();                               ///<Updates the GUI elements
      void reset_gui();                               ///<Resets the GUI elements
      std::string name;                               ///<Name of the probe displayed in this widget

    protected:

      Glib::RefPtr<Gtk::Builder> m_refBuilder;        ///<Gtk::Builder object of this widget

    private:

      swegui::MainWindow* parent = nullptr;           ///<Parent swegui::MainWindow
      std::string layer_names[5] = { "b", "h", "hu", "hv", "hx" };      ///<Helper array to access layer names
      Gtk::Label* labels[5];                                            ///<Pointer to Gtk::Labels showing the layer values in the GUI of this widget
      Gtk::ComboBox* cb_layer = nullptr;                                ///<Pointer to Gtk::Combobox, where a layer can be selected, which is then rendered in the graph
      Gtk::Button* btn_save_graph = nullptr;                            ///<Pointer to Gtk::Button to save a screenshot of the rendered graph
      Gtk::FileChooserDialog* sfd_save = nullptr;                       ///<Pointer to Gtk::FileChooserDialog, where hhe save location for a screenshot can be selected
      Gtk::DrawingArea* drawingarea_chart = nullptr;                    ///<Pointer to the Gtk::DrawingArea of the graph

      void setup_gui_elements();                                        ///<Initilizes the GUI elements

      void on_dataset_change(void);                                     ///<To be called whenever the dataset of the graph changes
      void on_graph_export(void);                                       ///<Event handler for screenshot export
      bool on_chart_draw(const Cairo::RefPtr<Cairo::Context>& cr);      ///<Refreshes the graph

      /**
       * @brief Calculates the y coordinate for rendering the graph
       * 
       * @param data Data point to be rendered
       * @param min Minimum value to be rendered
       * @param max Maximum value to be rendered
       * @param scale Ratio between graph data height and graph window height
       * @param graph_height Height of the graph window in pixels
       * 
       * @return Absolute y value of the datapoint in graph
      */
      float calculate_graph_height(float data, float min, float max, float scale, int graph_height);

      /**
       * @brief Calculates the x coordinate for rendering the graph
       * 
       * @param timestep Timestep of the value to be rendered
       * @param timesteps_total Total number of timestamps in dataset
       * @param graph_width Width of the graph window in pixels
       * 
       * @return Absolute x value of the datapoint in graph
      */
      float calculate_graph_width(int timestep, int timesteps_total, int graph_width);

      /**
       * @brief Saves a screenshot to the path specified in /p path
       * 
       * @param path Path to save the screenshot
      */
      void save_screenshot(std::string path);
  };

}

#endif