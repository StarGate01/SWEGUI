#include "SFMLWidgetGlade.hpp"
#include "SFMLWidget.hpp"
#include <gtkmm/main.h>

using namespace sfml;

void sfml_widgets_register()
{
    SFMLWidget::register_type();
}

extern "C" void sfml_widgets_glade_init()
{
    Gtk::Main::init_gtkmm_internals();
    sfml_widgets_register();
}