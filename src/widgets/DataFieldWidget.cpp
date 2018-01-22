#include <sstream>
#include <math.h>
#include "DataFieldWidget.hpp"
#include "../MainWindow.hpp"

using namespace widgets;

DataFieldWidget::DataFieldWidget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::Notebook(cobject), m_refBuilder(refBuilder)
{ }

DataFieldWidget* DataFieldWidget::create(swegui::MainWindow* pa, std::string na)
{
    auto refBuilder = Gtk::Builder::create_from_resource(PATH_TO_DATAFIELD_GUI);
    DataFieldWidget* widget = nullptr;
    refBuilder->get_widget_derived("notebook_main", widget);
    widget->parent = pa;
    widget->name = na;
    // widget->set_transient_for(*(widget->parent));
    widget->setup_gui_elements();
    return widget;
}

void DataFieldWidget::setup_gui_elements()
{
    //Raw data tab
    for(int i = 0; i < 5; i++)
        m_refBuilder->get_widget("lbl_"+layer_names[i], labels[i]);
    
    //Graph tab
    m_refBuilder->get_widget("cb_layer", cb_layer);

    update_ui();
}

void DataFieldWidget::update_ui()
{
    std::cout << "Updating ui" << std::endl;
    if(name != "")
    {
        probe::DataProbe* probe = &(parent->data_renderer->probes[name]);
        //Compute hx
        float hx = std::abs(std::sqrt(std::pow(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y), 2)
            + std::pow(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y), 2)));
        labels[0]->set_text(std::to_string(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::B, probe->x, probe->y)));
        labels[1]->set_text(std::to_string(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::H, probe->x, probe->y)));
        labels[2]->set_text(std::to_string(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y)));
        labels[3]->set_text(std::to_string(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hv, probe->x, probe->y)));
        labels[4]->set_text(std::to_string(hx));
    }
}