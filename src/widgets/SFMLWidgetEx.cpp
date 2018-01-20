#include "SFMLWidget.hpp"

using namespace widgets;

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

GType SFMLWidget::gtype = 0;

SFMLWidget::SFMLWidget(GtkBin* gobj)
    : Gtk::Bin(gobj)
{
    initialize(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT));
}

SFMLWidget::SFMLWidget()
    : Glib::ObjectBase("sfmlwidget")
{ 
    initialize(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT));
}

Glib::ObjectBase* SFMLWidget::wrap_new(GObject* o)
{
    if (gtk_widget_is_toplevel(GTK_WIDGET(o))) return new SFMLWidget(GTK_BIN(o));
    else return Gtk::manage(new SFMLWidget(GTK_BIN(o)));
}

void SFMLWidget::register_type()
{
    if (gtype) return;
    SFMLWidget dummy;
    GtkWidget *widget = GTK_WIDGET(dummy.gobj());
    gtype = G_OBJECT_TYPE(widget);
    Glib::wrap_register(gtype, SFMLWidget::wrap_new);
}
