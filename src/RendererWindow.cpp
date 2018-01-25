#include <iostream>
#include "RendererWindow.hpp"
#include "MainWindow.hpp"
#include "renderer/DataRenderer.hpp"

using namespace swegui;

RendererWindow::RendererWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder)
{ }

RendererWindow* RendererWindow::create(MainWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_RENDERER_GUI);
    RendererWindow* window = nullptr;
    refBuilder->get_widget_derived("window_renderer", window);
    window->parent = pa;
    window->set_transient_for(*(window->parent));
    window->setup_gui_elements();
    return window;
}

void RendererWindow::setup_gui_elements()
{
    for(int i=0; i<8; i++) 
    {
        m_refBuilder->get_widget("switch_" + switch_names[i], switches[i]);
        switches[i]->property_active().signal_changed().connect(sigc::mem_fun(this, &RendererWindow::on_action_switch));
    }
}

void RendererWindow::on_action_switch()
{
    parent->data_renderer->settings.probes = switches[0]->get_active();
    parent->data_renderer->settings.probe_names = switches[1]->get_active();
    parent->data_renderer->settings.probe_indicators = switches[2]->get_active();
    parent->data_renderer->settings.coordinates = switches[3]->get_active();
    parent->data_renderer->settings.zero = switches[4]->get_active();
    parent->data_renderer->settings.gizmo = switches[5]->get_active();
    parent->data_renderer->settings.scales = switches[6]->get_active();
    parent->data_renderer->settings.info = switches[7]->get_active();
    parent->data_renderer->invalidate();
}