#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>

class GuiApplicationWindow : public Gtk::ApplicationWindow
{
public:
    GuiApplicationWindow();
    void open_file_view(const Glib::RefPtr<Gio::File>& file);
};

#endif