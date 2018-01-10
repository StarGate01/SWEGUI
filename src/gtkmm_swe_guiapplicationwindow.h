#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>

class GuiApplicationWindow : public Gtk::ApplicationWindow
{
public:
    GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    
    static GuiApplicationWindow* create();

    void open_file_view(const Glib::RefPtr<Gio::File>& file);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
};

#endif