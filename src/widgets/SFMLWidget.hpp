#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <gtkmm.h>
#include <SFML/Graphics.hpp>

#define PATH_TO_SFML_GUI "/main/src/ui/sfml.glade"

namespace swegui
{

  class MainWindow;

}

namespace widgets
{

  class SFMLWidget : public Gtk::Bin
  {

    public:

      sf::RenderWindow renderWindow;

      SFMLWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
      static SFMLWidget* create(swegui::MainWindow* pa);

      void invalidate();
      void display();

    protected:

      Glib::RefPtr<Gtk::Builder> m_refBuilder;
      sf::VideoMode m_vMode;

      virtual void on_size_allocate(Gtk::Allocation &allocation);
      virtual void on_realize();
      virtual void on_unrealize();

      Glib::RefPtr<Gdk::Window> m_refGdkWindow;

    private:

      swegui::MainWindow* parent = nullptr;

      void setup_gui_elements();

  };

}

#endif