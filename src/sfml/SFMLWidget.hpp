#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gtk/gtkdrawingarea.h>
#include <SFML/Graphics.hpp>

namespace sfml
{

  class SFMLWidget : public Gtk::DrawingArea
  {

    private:

      static GType gtype;

      SFMLWidget(GtkDrawingArea *gobj);
      SFMLWidget();
      void initialize(sf::VideoMode mode, int size_request = -1);

      static Glib::ObjectBase *wrap_new(GObject *o);

    protected:

      sf::VideoMode m_vMode;

      virtual void on_size_allocate(Gtk::Allocation &allocation);
      virtual void on_realize();
      virtual void on_unrealize();

      Glib::RefPtr<Gdk::Window> m_refGdkWindow;

    public:

      sf::RenderWindow renderWindow;

      SFMLWidget(sf::VideoMode mode, int size_request = -1);
      virtual ~SFMLWidget();

      void invalidate();
      void display();

      static void register_type();
      
  };

}

#endif