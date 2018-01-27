#ifndef DATAFIELD_WIDGET_H
#define DATAFIELD_WIDGET_H

#include <string>
#include <gtkmm.h>
#include <gtkmm/messagedialog.h>
#include <pangomm/attrlist.h>   //Set attributes for labels
#include <cairomm/context.h>

#define PATH_TO_DATAFIELD_GUI "/main/src/ui/datafield.glade"

#define GRAPH_SCALE 0.7
#define COLOR_WATER "#005ce6"
#define COLOR_SPEED1 "#bbff99"
#define COLOR_SPEED2 "#aaff80"
#define COLOR_SPEED3 "#99ff66"
#define COLOR_DEFAULT "#d3d3d3"
#define LEGEND_FONT_SIZE 10
#define LEGEND_H_DEVISION 6
#define LEGEND_V_MAX_DEVISION 10
#define MIN_OFFSET 0.15

namespace swegui
{

  class MainWindow;

}

namespace widgets
{
  class DataFieldWidget : public Gtk::Notebook
  {

    public:

      DataFieldWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
      static DataFieldWidget* create(swegui::MainWindow* pa, std::string na);

      void update_ui();
      void reset_gui();
      std::string name;

    protected:

      Glib::RefPtr<Gtk::Builder> m_refBuilder;

    private:

      swegui::MainWindow* parent = nullptr;
      std::string layer_names[5] = { "b", "h", "hu", "hv", "hx" };
      Gtk::Label* labels[5];
      Gtk::ComboBox* cb_layer = nullptr;
      Gtk::Button* btn_save_graph = nullptr;
      Gtk::FileChooserDialog* sfd_save = nullptr;
      Gtk::DrawingArea* drawingarea_chart = nullptr;

      void setup_gui_elements();
      void populate_data();

      void on_dataset_change(void);
      void on_graph_export(void);
      bool on_chart_draw(const Cairo::RefPtr<Cairo::Context>& cr);

      float calculate_legend_value(float x, float min, float max, float scale, int graph_height);
      float calculate_graph_height(float data, float min, float max, float scale, int graph_height);
      float calculate_graph_width(int timestep, int timesteps_total, int graph_width);

      bool save_screenshot(std::string path);
  };

}

#endif