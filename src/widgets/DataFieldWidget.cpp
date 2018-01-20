#include <iostream>
#include <cstdlib>
#include "DataFieldWidget.hpp"
#include <gdk/gdkx.h>
#include <unistd.h>

using namespace widgets;
using namespace std;

void DataFieldWidget::initialize()
{
    set_size_request(100, 100);
    set_has_window(false);
    // m_refBuilder = Gtk::Builder::create_from_resource(PATH_TO_DATAFIELD_GUI);
    // m_refBuilder->get_widget("notebook_main", notebook);
    // add(*notebook);
    // notebook->set_visible();
}