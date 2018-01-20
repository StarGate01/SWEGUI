#include "WidgetsGlade.hpp"
#include "SFMLWidget.hpp"
#include <gtkmm/main.h>

using namespace widgets;

void widgets_register()
{
    SFMLWidget::register_type();
}

extern "C" void widgets_glade_init()
{
    Gtk::Main::init_gtkmm_internals();
    widgets_register();
}