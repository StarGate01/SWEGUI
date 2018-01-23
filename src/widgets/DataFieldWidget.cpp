#include <iostream>
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
    
    m_refBuilder->get_widget("cb_layer", cb_layer);
    m_refBuilder->get_widget("drawingarea_chart", drawingarea_chart);

    drawingarea_chart->signal_draw().connect(sigc::mem_fun(this, &DataFieldWidget::on_chart_draw));
    //todo bind combobox change event

    update_ui();
}

void DataFieldWidget::populate_data()
{
    //todo fill presampled array thingy
}

void DataFieldWidget::update_ui()
{
    if(name != "")
    {
        probe::DataProbe* probe = &(parent->data_renderer->probes[name]);
        
        for(int i=0; i<4; i++)
            labels[i]->set_text(std::to_string(parent->data_renderer->sample((renderer::NetCdfImageStream::Variable)i, probe->x, probe->y)));

        //Compute hx
        float hx = std::abs(std::sqrt(std::pow(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y), 2)
            + std::pow(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hu, probe->x, probe->y), 2)));
        labels[4]->set_text(std::to_string(hx));
    }
    else
    {
        reset_gui();
    }
}

void DataFieldWidget::reset_gui()
{
    for(auto label : labels)
        label->set_text("n/a");
}

bool DataFieldWidget::on_chart_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    //todo use cr to draw graph, but only if data array is filled
    //based on what is selected in combobox
    return true;
}