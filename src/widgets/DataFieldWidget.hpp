#ifndef DATAFIELD_WIDGET_H
#define DATAFIELD_WIDGET_H

#include <string>
#include <gtkmm.h>

#define PATH_TO_DATAFIELD_GUI "/main/src/ui/datafield.glade"

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
      std::string name;

    protected:

      Glib::RefPtr<Gtk::Builder> m_refBuilder;

    private:

      swegui::MainWindow* parent = nullptr;

      void setup_gui_elements();
      
  };

}

#endif