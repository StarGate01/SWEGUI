#ifndef GTKMM_GUIAPPLICATIONWINDOW
#define GTKMM_GUIAPPLICATIONWINDOW

#include <gtkmm.h>

class GuiApplicationWindow : public Gtk::ApplicationWindow
{
    GuiApplicationWindow();

    void open_file_view(const Glib::RefPtr<Gui::File>& file);
};

#endif