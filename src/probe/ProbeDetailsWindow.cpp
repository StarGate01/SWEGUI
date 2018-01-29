/**
 * @file ProbeDetailsWindow.cpp
 * @brief Implements the functionality defined in ProbeDetailsWindow.hpp
*/

#include "ProbeDetailsWindow.hpp"
#include "../MainWindow.hpp"

using namespace probe;

ProbeDetailsWindow::ProbeDetailsWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder)
{ }

ProbeDetailsWindow* ProbeDetailsWindow::create(swegui::MainWindow* pa, std::string na)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_PROBE_DETAIL_GUI);
    ProbeDetailsWindow* window = nullptr;
    refBuilder->get_widget_derived("window_probe", window);
    window->parent = pa;
    window->name = na;
    window->set_title(na + " Details");
    window->set_transient_for(*(window->parent));
    window->setup_gui_elements();
    return window;
}

void ProbeDetailsWindow::setup_gui_elements()
{
    m_refBuilder->get_widget("window_probe", window_probe);
    probedata = widgets::DataFieldWidget::create(parent, name);
    window_probe->add(*probedata);
}

void ProbeDetailsWindow::update_ui()
{
    probedata->update_ui();
}