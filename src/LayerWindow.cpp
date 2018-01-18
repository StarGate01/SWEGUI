#include "LayerWindow.hpp"
#include "GuiApplicationWindow.hpp"

using namespace swegui;

LayerWindow::LayerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder)
{
    setup_gui_elements();
}

LayerWindow* LayerWindow::create(GuiApplicationWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_file(PATH_TO_LAYER_GUI);
    LayerWindow* window = nullptr;
    refBuilder->get_widget_derived("window_layers", window);
    window->parent = pa;
    window->set_transient_for(*(window->parent));
    return window;
}

void LayerWindow::setup_gui_elements()
{
    m_refBuilder->get_widget("switch_b", switch_b);
    m_refBuilder->get_widget("switch_h", switch_h);
    m_refBuilder->get_widget("switch_hu", switch_hu);
    m_refBuilder->get_widget("switch_hv", switch_hv);
    m_refBuilder->get_widget("switch_hx", switch_hx);

    switch_b->property_active().signal_changed().connect(sigc::mem_fun(this, &LayerWindow::on_layer_switch_changed));
    switch_h->property_active().signal_changed().connect(sigc::mem_fun(this, &LayerWindow::on_layer_switch_changed));
    switch_hu->property_active().signal_changed().connect(sigc::mem_fun(this, &LayerWindow::on_layer_switch_changed));
    switch_hv->property_active().signal_changed().connect(sigc::mem_fun(this, &LayerWindow::on_layer_switch_changed));
    switch_hx->property_active().signal_changed().connect(sigc::mem_fun(this, &LayerWindow::on_layer_switch_changed));
}

void LayerWindow::on_layer_switch_changed()
{
    parent->data_renderer->b.enable = switch_b->get_active();
    parent->data_renderer->h.enable = switch_h->get_active();
    parent->data_renderer->hu.enable = switch_hu->get_active();
    parent->data_renderer->hv.enable = switch_hv->get_active();
    parent->data_renderer->hx.enable = switch_hx->get_active();
    parent->data_renderer->update_shader();
}