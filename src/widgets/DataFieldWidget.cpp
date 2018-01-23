#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <limits>
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
    m_refBuilder->get_widget("liststore_cb", cb_layer_model);
    m_refBuilder->get_widget("drawingarea_chart", drawingarea_chart);

    drawingarea_chart->signal_draw().connect(sigc::mem_fun(this, &DataFieldWidget::on_chart_draw));
    cb_layer->signal_changed().connect(sigc::mem_fun(this, &DataFieldWidget::on_dataset_change));           //TODO: Add parameter to function

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
            + std::pow(parent->data_renderer->sample(renderer::NetCdfImageStream::Variable::Hv, probe->x, probe->y), 2)));
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

void DataFieldWidget::on_dataset_change(void)
{
    //TODO: Call DataFieldWidet::on_chart_draw
    std::cout << "Selected " << cb_get_selected_layer() << std::endl;
}

bool DataFieldWidget::on_chart_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{ 
    probe::DataProbe* probe = &(parent->data_renderer->probes[name]);
    //Check if dataset is filled
    if(!probe->has_data())
        return true;
    
    //TODO: Calculate graph width and height
    int width = 0;
    int height = 0;
    int layer = cb_get_selected_layer();
    std::vector<float> data = probe->get_all_data(layer);

    if(data.size() <= 0)
        return true;

    float max_value = std::numeric_limits<float>::min();
    //Get maximum element
    for(auto const& val : data)
    {
        if(max_value < val)
            max_value = val;
    }

    if(data.size() == 1)
    {
        cr->move_to(calculate_graph_width(0, static_cast<int>(data.size()), width), height/2);
        cr->line_to(width, height/2);
        return true;
    }

    cr->move_to(calculate_graph_width(0, static_cast<int>(data.size()), width), calculate_graph_height(data[0], max_value, GRAPH_SCALE, height));
    
    //remove first item from dataset
    data.erase(data.begin());

    //Iterate though remaining items
    for(int t = 0; t < static_cast<int>(data.size()); t++)
        cr->line_to(calculate_graph_width(t, static_cast<int>(data.size())+1, width), calculate_graph_height(data[t], max_value, GRAPH_SCALE, height));
    
    return true;
}

int DataFieldWidget::cb_get_selected_layer()
{
    const std::string val = cb_layer->get_active_id();
    for(int i = 0; i < 5; i++)
        if(val.compare(layer_names[i]) == 0) return i;
   return -1;
}

float DataFieldWidget::calculate_graph_height(float data, float max, float scale, int graph_height)
{
    return (data / max) * scale * (float) graph_height;
}

float DataFieldWidget::calculate_graph_width(int timestep, int timesteps, int graph_width)
{
    return ((float) timestep / (float)timesteps)* (float) graph_width;
}