#include <iostream>
#include "LayerWindow.hpp"
#include "MainWindow.hpp"
#include "renderer/Layer.hpp"
#include "renderer/DataRenderer.hpp"

using namespace swegui;

LayerWindow::LayerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Window(cobject), m_refBuilder(refBuilder)
{ }

LayerWindow::~LayerWindow()
{
    for(int i=0; i<5; i++) delete layer_uis[i];
}

LayerWindow* LayerWindow::create(MainWindow* pa)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_LAYER_GUI);
    LayerWindow* window = nullptr;
    refBuilder->get_widget_derived("window_layers", window);
    window->parent = pa;
    window->set_transient_for(*(window->parent));
    window->setup_gui_elements();
    return window;
}

void LayerWindow::setup_gui_elements()
{
    for(int i=0; i<5; i++)
    {
        std::string name = layer_names[i];
        LayerUI* layer = new LayerUI();
        layer->data_layer = parent->data_renderer->layers[i];
        m_refBuilder->get_widget("switch_" + name,  layer->switch_enable);
        m_refBuilder->get_widget("switch_" + name + "_clip",  layer->switch_clip);
        m_refBuilder->get_widget("spin_" + name + "_min",  layer->spin_min);
        m_refBuilder->get_widget("spin_" + name + "_max",  layer->spin_max);
        layer->adjustment_min = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_" + name + "_min"));
        layer->adjustment_max = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(m_refBuilder->get_object("adjustment_" + name + "_max"));
        layer->switch_enable->property_active().signal_changed().connect(sigc::mem_fun(layer, &LayerWindow::LayerUI::update_data));
        layer->switch_clip->property_active().signal_changed().connect(sigc::mem_fun(layer, &LayerWindow::LayerUI::update_data));
        layer->spin_min->signal_value_changed().connect(sigc::mem_fun(layer, &LayerWindow::LayerUI::update_data));
        layer->spin_max->signal_value_changed().connect(sigc::mem_fun(layer, &LayerWindow::LayerUI::update_data));
        layer->signal_update().connect(sigc::mem_fun(parent->data_renderer, &renderer::DataRenderer::invalidate));
        layer_uis[i] = layer;
    }
    update_ui();
}

void LayerWindow::update_ui()
{
    for(int i=0; i<5; i++) layer_uis[i]->update_ui();
}

LayerWindow::LayerUI::type_signal_update LayerWindow::LayerUI::signal_update()
{
    return m_signal_update;
}

void LayerWindow::LayerUI::update_data()
{
    data_layer->enable = switch_enable->get_active();
    data_layer->clip = switch_clip->get_active();
    data_layer->clip_min = spin_min->get_value();
    data_layer->clip_max = spin_max->get_value();
    data_layer->update_shader();
    m_signal_update.emit();
}

void LayerWindow::LayerUI::update_ui()
{
    switch_enable->set_active(data_layer->enable);
    switch_enable->set_state(data_layer->enable);
    switch_clip->set_active(data_layer->clip);
    switch_clip->set_state(data_layer->clip);
    adjustment_min->set_lower(data_layer->meta_info.min);
    adjustment_min->set_upper(data_layer->meta_info.max);
    adjustment_max->set_lower(data_layer->meta_info.min);
    adjustment_max->set_upper(data_layer->meta_info.max);
    spin_min->set_value(data_layer->clip_min);
    spin_max->set_value(data_layer->clip_max);
}