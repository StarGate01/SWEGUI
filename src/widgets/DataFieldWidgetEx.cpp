#include <iostream>
#include "DataFieldWidget.hpp"

using namespace widgets;
using namespace std;

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
    DataFieldWidget* widget = new DataFieldWidget(GTK_BIN(o));
    if (gtk_widget_is_toplevel(GTK_WIDGET(o))) return widget;
    else return Gtk::manage(widget);
}

void DataFieldWidget::register_type()
{
    if (gtype) return;
    DataFieldWidget dummy;
    GtkWidget *widget = GTK_WIDGET(dummy.gobj());
    gtype = G_OBJECT_TYPE(widget);
    Glib::wrap_register(gtype, DataFieldWidget::wrap_new);
}
