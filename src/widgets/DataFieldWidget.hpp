#ifndef DATAFIELD_WIDGET_H
#define DATAFIELD_WIDGET_H

#include <string>
#include <gtkmm.h>
#include <gtkmm/bin.h>
#include <gtk/gtkbin.h>

#define PATH_TO_DATAFIELD_GUI "/widgets/src/widgets/ui/datafield.glade"

namespace widgets
{

  class DataFieldWidget : public Gtk::Bin
  {

    public:

      DataFieldWidget();
      virtual ~DataFieldWidget() { }

      static void register_type();

    protected:

      virtual void on_realize();
      virtual void on_unrealize();

      Glib::RefPtr<Gtk::Builder> m_refBuilder;

    private:

      static GType gtype;

      DataFieldWidget(GtkBin *gobj);
      void initialize();

      static Glib::ObjectBase *wrap_new(GObject *o);

      Gtk::Notebook* notebook = nullptr;
      
  };

}

#endif