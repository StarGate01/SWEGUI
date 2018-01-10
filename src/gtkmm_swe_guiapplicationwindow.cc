#include <stdexcept>
#include <gtkmm.h>
#include "gtkmm_swe_guiapplicationwindow.h"


GuiApplicationWindow::GuiApplicationWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder)
{
}

GuiApplicationWindow* GuiApplicationWindow::create()
{
    //Load builder and instantiate widget
    auto refBuilder = Gtk::Builder::create_from_file("ui/main.gtk");

    GuiApplicationWindow* window = nullptr;
    refBuilder->get_widget_derived("window_main", window);
    if(!window)
    {
        throw std::runtime_error("window_main not found in ui/main.gtk");
    }

    return window;
}

void GuiApplicationWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
    
}