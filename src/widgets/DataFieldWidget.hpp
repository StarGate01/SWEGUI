#ifndef DATAFIELD_WIDGET_H
#define DATAFIELD_WIDGET_H

#include <gtkmm.h>
#include <gtkmm/bin.h>
#include <gtk/gtkbin.h>

namespace widgets
{

  class DataFieldWidget : public Gtk::Bin
  {

    private:

      static GType gtype;

      DataFieldWidget(GtkBin *gobj);
      void initialize();

      static Glib::ObjectBase *wrap_new(GObject *o);

    protected:


    public:

      DataFieldWidget();
      virtual ~DataFieldWidget();

      static void register_type();
      
  };

}

#endif