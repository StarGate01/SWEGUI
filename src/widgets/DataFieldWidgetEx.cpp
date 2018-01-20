#include "DataFieldWidget.hpp"

using namespace widgets;

GType DataFieldWidget::gtype = 0;

DataFieldWidget::DataFieldWidget(GtkBin* gobj)
    : Gtk::Bin(gobj)
{
    initialize();
}

DataFieldWidget::DataFieldWidget()
    : Glib::ObjectBase("datafieldwidget")
{ 
    initialize();
}

Glib::ObjectBase* DataFieldWidget::wrap_new(GObject* o)
{
    if (gtk_widget_is_toplevel(GTK_WIDGET(o))) return new DataFieldWidget(GTK_BIN(o));
    else return Gtk::manage(new DataFieldWidget(GTK_BIN(o)));
}

void DataFieldWidget::register_type()
{
    if (gtype) return;
    DataFieldWidget dummy;
    GtkWidget *widget = GTK_WIDGET(dummy.gobj());
    gtype = G_OBJECT_TYPE(widget);
    Glib::wrap_register(gtype, DataFieldWidget::wrap_new);
}
