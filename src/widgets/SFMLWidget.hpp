#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <gtkmm.h>
#include <gtkmm/bin.h>
#include <gtk/gtkbin.h>
#include <SFML/Graphics.hpp>

namespace widgets
{

  class SFMLWidget : public Gtk::Bin
  {

    public:

      sf::RenderWindow renderWindow;

      SFMLWidget(sf::VideoMode mode, int size_request = -1);
      virtual ~SFMLWidget() { }

      void invalidate();
      void display();

      static void register_type();

    protected:

      sf::VideoMode m_vMode;

      virtual void on_size_allocate(Gtk::Allocation &allocation);
      virtual void on_realize();
      virtual void on_unrealize();

      Glib::RefPtr<Gdk::Window> m_refGdkWindow;

    private:

      static GType gtype;

      SFMLWidget(GtkBin *gobj);
      SFMLWidget();
      void initialize(sf::VideoMode mode, int size_request = -1);

      static Glib::ObjectBase *wrap_new(GObject *o);

  };

}

#endif