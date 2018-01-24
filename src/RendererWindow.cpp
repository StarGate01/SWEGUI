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
    m_refBuilder->get_widget("switch_probes", switch_probes);
    m_refBuilder->get_widget("switch_names", switch_names);
    m_refBuilder->get_widget("switch_indicators", switch_indicators);
    m_refBuilder->get_widget("switch_info", switch_info);

    switch_probes->property_active().signal_changed().connect(sigc::mem_fun(this, &RendererWindow::on_action_switch));
    switch_names->property_active().signal_changed().connect(sigc::mem_fun(this, &RendererWindow::on_action_switch));
    switch_indicators->property_active().signal_changed().connect(sigc::mem_fun(this, &RendererWindow::on_action_switch));
    switch_info->property_active().signal_changed().connect(sigc::mem_fun(this, &RendererWindow::on_action_switch));
}

void RendererWindow::on_action_switch()
{
    parent->data_renderer->render_probes = switch_probes->get_active();
    parent->data_renderer->render_probe_names = switch_names->get_active();
    parent->data_renderer->render_probe_arrows = switch_indicators->get_active();
    parent->data_renderer->render_info = switch_info->get_active();
    parent->data_renderer->invalidate();
}