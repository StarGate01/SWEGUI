#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gtk/gtkdrawingarea.h>

class SFMLWidget : public Gtk::DrawingArea
{

  private:

    static GType gtype;

    SFMLWidget(GtkDrawingArea *gobj);
    SFMLWidget();

    static Glib::ObjectBase *wrap_new(GObject *o);

  public:

    static void register_type();
    
};

#endif