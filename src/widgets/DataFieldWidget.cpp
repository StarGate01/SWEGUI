#include "DataFieldWidget.hpp"
#include <gdk/gdkx.h>

using namespace widgets;

void DataFieldWidget::initialize()
{
    set_size_request(100, 100);
    set_has_window(false);
}

DataFieldWidget::~DataFieldWidget()
{ }